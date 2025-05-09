/*
	MIT License

	Copyright (c) 2025 rollinTaters

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:
	
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/


#include <chrono>
#include <iostream> // cout cerr
#include "ngc.hpp"

// == public: ==

NGC::NGC( Vehicle* vehicle ): m_comms_module(CommsModule::udp, CommsModule::ngc_channel)
{
    // assign controlled vehicle
    m_vehicle = vehicle;

};

NGC::~NGC()
{
    stop();
    stopDeadReckoning();
}

bool NGC::start()
{
    // start necessary threads and do runtime initializations
    motor_R.startControlLoop();
    motor_L.startControlLoop();

    // check if we are already runnng the thread
    if( m_main_thread != nullptr )
    {
        std::cerr<<"NGC: Warning: Tried to start main thread when it is already running.\n";
        return false;
    }
    std::cout<<"NGC: Starting main thread\n";
    m_run_main_thread = true;
    m_main_thread = new std::thread( &NGC::mainThreadFunc, this );
    return true;
}

bool NGC::stop()
{
    // send stop signals and join the threads here
    motor_R.stopControlLoop();
    motor_L.stopControlLoop();

    // check if we have a thread to stop
    if( m_main_thread == nullptr )
    {
        std::cerr<<"NGC: Warning: Tried to stop main thread. We dont have a main thread to stop.\n";
        return false;
    }
    std::cout<<"NGC: Stopping main thread\n";
    // signal main thread to stop
    m_run_main_thread = false;
    m_main_thread->join();
    delete m_main_thread;
    m_main_thread = nullptr;

    // TODO is there a fail condition?? can the thread not join? what then?
    return true;
}

bool NGC::startDeadReckoning()
{
    m_last_dead_reckon_time = m_clock.now();
    if( m_dead_reckoning_thread != nullptr )
    {
        //std::cerr<<"NGC: Warning: Dead reckoning thread is already running.\n";
        return false;
    }
    m_run_dead_reckoning_thread = true;
    m_dead_reckoning_thread = new std::thread( &NGC::deadReckonFunc, this );
    return true;
}

bool NGC::stopDeadReckoning()
{
    if( m_dead_reckoning_thread == nullptr )
    {
        //std::cerr<<"NGC: Warning: Tried to stop dead reckoning thread, there is no thread.\n";
        return false;
    }
    m_run_dead_reckoning_thread = false;
    m_dead_reckoning_thread->join();
    delete m_dead_reckoning_thread;
    m_dead_reckoning_thread = nullptr;
    return true;
}

bool NGC::addWP( Point wp )
{
    m_waypoints.push_back( wp );
    return true;
}

bool NGC::addWP( Point wp, int i_num )
{
    if( i_num > m_waypoints.size() )
        return false;
    m_waypoints.insert( m_waypoints.begin()+i_num, wp );
    return true;
}

std::vector<Point> NGC::getWPs() const
{
    return m_waypoints;
}

bool NGC::executeWPs()
{
    if( m_waypoints.empty() )
    {
        m_execute_waypoints = false;
        return false;
    }
    m_execute_waypoints = true;
    return true;
}

std::vector<Point> NGC::getImObPoints() const { return m_immediate_obstacles; }

void NGC::directCommand( float speed, float rate )
{
    // override execution status to stop guidance methods from inferring with manual user input
    m_execute_waypoints = false;
    setControlOutput_rate( speed, rate );
}

// == private: ==

void NGC::mainThreadFunc()
{
    // on first start, check if dead reckoning is active
    if( startDeadReckoning() )
        std::cout<<"Starting dead reckoning\n";

    //int counter = 0;
    while( m_run_main_thread )
    {
        // roll rol roll
        // check if there are any incoming packets
        if( m_comms_module.packetAvailable() )
        {
            m_comms_module.readPacket( m_command_packet );
            processPacket( m_command_packet );
        }
        // TODO send telemetry back
        // sendTelemetry();

        // TODO check if dead reckoning is still active??

        // TODO run predictTrajectory and send it to command console for debug visualization

        // read lidar and run markImmediateObstacles
        getLIDARData();

        // TODO run createTargetWaypoint, createOpenSpaceWaypoint

        // TODO run CONTROL type methods

        // checking wp satisfaction
        if( !m_waypoints.empty() && m_waypoints.front().absDist(m_vehicle->getBox().getPos()) < 0.1f )
        {
            m_waypoints.erase( m_waypoints.begin() );
        }

        if( !m_waypoints.empty() && m_execute_waypoints )
        {
            hitWP( m_waypoints.front() );
        }else{
            m_execute_waypoints = false;
            halt();
        }

        //std::cout<<"ngc main thread spam. counter:"<<counter<<"\n";
        //counter++;
        std::this_thread::sleep_for( std::chrono::milliseconds(200) );
    }
    // DEBUG
    std::cout<<"NGC: Main thread exiting\n";
}

bool NGC::getLIDARData()
{
    // do a lidar read first, we are doin it manually here for now
    m_vehicle->readSensor(1);

    // sensor 0 -> IMU, 1-> LIDAR
    BB3D sensor_box = m_vehicle->getSensor(1).getBox();
    
    v3f sensor_position = sensor_box.getPos();
    v3f sensor_direction = sensor_box.getLocalVecY();
    cQuaternion lidar_quat = cQuaternion::fromAxisAngle( sensor_box.getLocalVecZ(), 0 );
    Sensor_Data data = m_vehicle->getSensorData();

    // clear previous read
    m_immediate_obstacles.clear();

    for( int i = 0; i < LIDAR_POINTS; i++ )
    {
        if( data.lidar[i] == 0.f ) continue;    // no read is 0 metres
        lidar_quat.w = data.lidar_angle[i] / 2.f;
        Point p = sensor_direction * data.lidar[i];
        lidar_quat.rotateVector( p );
        p += sensor_position;
        m_immediate_obstacles.push_back( p );
    }
    // maybe return false if the quality of the sensor read is low
    return true;
}

void NGC::deadReckonFunc()
{
    using namespace std::chrono;
    std::chrono::milliseconds time_step;
    while( m_run_dead_reckoning_thread )
    {
        // check if its time to do it
        if( m_clock.now() < m_last_dead_reckon_time + m_dead_reckoning_interval )
        {
            // nope, not now
            std::this_thread::sleep_for( m_dead_reckoning_interval /5 );
            continue;
        }else{
            time_step = duration_cast<milliseconds>(steady_clock::now() - m_last_dead_reckon_time);
        }

        // make sure the vehicles internal IMU sensor data is up to date
        // NOTE: sensor[0] is always the IMU (i just decided that)
        // request IMU sensor read
        m_vehicle->readSensor(0);

        // immediately update time point
        m_last_dead_reckon_time = m_clock.now();

        // run vehicles setNavigationState, which actually does the dead reckoning calculations
        m_vehicle->setNavigationState( time_step.count() );

        // TODO this time step should be reported as a quality measure to the command console
    }
}

bool NGC::markObstacles()
{
    return false;
}

bool NGC::markImmediateObstacles()
{
    return false;
}

bool NGC::predictTrajectory()
{
    return false;
}

bool NGC::createTargetWaypoint()
{
    return false;
}

bool NGC::createOpenSpaceWaypoint( Point& start_point )
{
    // output waypoint will be at least this distance away from any obstacles
    float avoid_radius = 1.f; // metre 

    float target_seperation = 1.5f; // metre

    // search for obstacles (LIDAR points) in this radius from starting point
    float search_radius = 4.f; // metre

    std::vector<Point> relevant_points;

    // TODO maybe a "merge" filter, that merges two points that are closer than a threshold together

    // find lidar points in a given radius
    for( Point point : m_immediate_obstacles )
    {
        if( point.absDist( start_point ) <= search_radius )
            relevant_points.push_back( point );
    }

    // Iterating to find an output point that satisfies given seperation with relevant points
    // iteration controls
    unsigned int current_it = 0;
    unsigned int max_it = 100;
    float target_mean_sq_err = 0.5f;

    // iteration variables
    float total_sq_err = 0;
    float mean_sq_err = 0;
    Point out_point = start_point;
    Point total_sq_err_vector;

    // nudge output point to be seperation distance away from relevant points
    while( true )
    {
        for( Point p : relevant_points )
        {
            // find square of error
            float sq_err = out_point.sqErrSep( p, target_seperation );

            // add it to total squared error accumulator
            total_sq_err += sq_err;

            // find error direction
            Point sq_err_vector = out_point - p;

            // make it a unit vector
            sq_err_vector = sq_err_vector.unit();

            // scale unit vector by squared error amount
            sq_err_vector *= sq_err;

            // add it to total squared error vector
            total_sq_err_vector += sq_err_vector;
        }
        mean_sq_err = total_sq_err / relevant_points.size();

        // nudge output point by total squared error vector
        out_point += total_sq_err_vector * 0.5f;    // FIXME is this + or -

        if( mean_sq_err < target_mean_sq_err )
        {   // success
            break;
        }
        // iteration status checks
        if( current_it > max_it )
        {   // fail
            std::cerr<<"NGC: createOpenSpaceWaypoint method exeded max iteration limit ("
                <<current_it<<"/"<<max_it<<")\n";
            std::cerr<<"---: mse: "<<mean_sq_err<<" | target mse: "<<target_mean_sq_err<<"\n";
            return false;
        }
        // increment iteration counter
        current_it++;
    }

    // TODO make sure the output point satisfies avoid radius
    // TODO make sure output point is xx distance above the "ground"

    // return the calculated point
    start_point = out_point;
    return true;
}

int NGC::hitWP( Point wp )
{
    int mode = -1;

    // TODO what is our target speed for the part of the course, get that from the CCM
    float target_speed = 1.2; // m/s
    float target_reverse_speed = -0.50;   // m/s
    float point_turn_rate = 10 * (PI/180.f);    // rad/s

    float t_speed = 0;  // will be set accordingly
    float t_radius = 0; // will be calculated if needed
    BB3D box = m_vehicle->getBox();
    

    // delta between target wp and vehicle position
    Point d_wp( wp.x - box.getPos().x,
                wp.y - box.getPos().y,
                wp.z - box.getPos().z );

    // wp's heading
    float wp_heading = d_wp.heading();

    // our vehicles heading
    float vehicle_heading = -box.getAngEuler().z;    // x: attitude y: bank z: heading

    // wp's bearing (aka target bearing)
    float t_bearing = wp_heading - vehicle_heading;

    // make sure bearing is between 0 and 2pi
    t_bearing += 2*PI;
    t_bearing = fmod( t_bearing, (2*PI) );
    
    /*
    std::cout<<"\n";
    std::cout<<"NGC: hitWP(): delta wp: "<<d_wp.x<<"x "<<d_wp.y<<"y "<<d_wp.z<<"z\n";
    std::cout<<"NGC: hitWP(): wp heading: "<<wp_heading<<"\n";
    std::cout<<"NGC: hitWP(): delta bearing: "<<t_bearing<<"\n";
    */

    // front and rear mobility cones
    // 0-5 degrees -> ignore turn, just go straight
    constexpr float no_turn_deg = 5;
    // 5-30 degrees -> do regular arc turn
    constexpr float arc_turn_deg = 30;
    // 30 - 90 degrees -> do point turn, then straight line move
    // same shit is mirrored for reverse operations

    constexpr float fwd_r_nt = no_turn_deg * (PI/180);
    constexpr float fwd_l_nt = (2*PI) - (no_turn_deg * (PI/180));
    constexpr float fwd_r_at = arc_turn_deg * (PI/180);
    constexpr float fwd_l_at = (2*PI) - (arc_turn_deg * (PI/180));
    
    constexpr float bcw_r_nt = fwd_l_nt - PI;
    constexpr float bcw_l_nt = fwd_r_nt + PI;
    constexpr float bcw_r_at = fwd_l_at - PI;
    constexpr float bcw_l_at = fwd_r_at + PI;

    /*// DEBUG 
    std::cout<<"t_bearing: "<<t_bearing<<"\n"<<
        "cones:\n"
        "1-fwd r  no turn: "<< fwd_r_nt<<"\n"<<
        "2-fwd r arc turn: "<< fwd_r_at<<"\n"<<
        "3-  R point turn: "<< bcw_r_at<<"\n"<<
        "4-bcw r arc turn: "<< bcw_r_nt<<"\n"<<
        "5-bcw rl no turn: "<< bcw_l_nt<<"\n"<<
        "6-bcw l arc turn: "<< bcw_l_at<<"\n"<<
        "7-fwd l pnt turn: "<< fwd_l_at<<"\n"<<
        "8-fwd l arc turn: "<< fwd_l_nt<<"\n";
    */

    // find out in which cone we are
    if( t_bearing <= fwd_r_nt || t_bearing >= fwd_l_nt )
    {   // forward no turn
        //std::cout<<"mode: forward no turn\n";
        setControlOutput_rate( target_speed, 0 );
        return 1;

    }else if( t_bearing <= fwd_r_at )
    {   // forward right arc turn
        //std::cout<<"mode: forward right arc turn\n";
        t_speed = target_speed;
        mode = 2;

    }else if( t_bearing <= bcw_r_at )
    {   // Point turn right
        //std::cout<<"mode: forward point turn right\n";
        setControlOutput_rate( 0, -point_turn_rate );
        return 3;

    }else if( t_bearing <= bcw_r_nt )
    {   // backward right arc turn
        //std::cout<<"mode: backward right arc turn\n";
        t_speed = target_reverse_speed;
        mode = 4;

    }else if( t_bearing <= bcw_l_nt )
    {   // backward no turn
        //std::cout<<"mode: backward no turn\n";
        setControlOutput_rate( target_reverse_speed, 0 );
        return 5;

    }else if( t_bearing <= bcw_l_at )
    {   // backward left arc turn
        //std::cout<<"mode: backward left arc turn\n";
        t_speed = target_reverse_speed;
        mode = 6;

    }else if( t_bearing <= fwd_l_at )
    {   // forward left point turn
        //std::cout<<"mode: forward point turn left\n";
        setControlOutput_rate( 0, point_turn_rate );
        return 7;

    }else //if( t_bearing <= fwd_l_nt )
    {   // forward left arc turn
        //std::cout<<"mode: forward left arc turn\n";
        t_speed = target_speed;
        mode = 8;
    }


    // -- setting turn radius as necessary --

    // with some math, we find that angle of rotation from arc center is 2*bearing
    // this also means that we will have a heading change of this same 2*bearing value

    // using cosine theorem: c^2 = a^2 + b^2 - 2ab(cos(C))
    // d_wp.mag()^2 = 2(t_radius^2) - 2(t_radius^2)*cos(2*t_bearing)
    // d_wp.mag()^2 = (2(t_radius^2)) * (1-cos(2*t_bearing)) 
    // (d_wp.mag()^2) / (1-cos(2*t_bearing) = 2*(t_radius^2)
    // t_radius^2 = (d_wp.mag()^2) / ((1-cos(2*t_bearing)*2)
    t_radius = sqrt( (pow(d_wp.mag(),2)) / (2.f*(1.f-cos(2*t_bearing))) );

    // t_radius should have a sign determining the left or right handedness of turn
    if( t_bearing > PI ) // port side, sign should be (-)
        t_radius *= -1.f;

    // -- setting target speed as necessary --

    // send a "target speed" signal to drive motor controller program
    setControlOutput_radius( t_speed, t_radius );
    return mode;
}

void NGC::halt()
{
    motor_R.setSpeed(0);
    motor_L.setSpeed(0);
}

void NGC::setControlOutput_rate( float speed, float turn_rate )
{
    float track_width = m_vehicle->track;   // metre

    // a crude way of calculating speed difference between sides
    float diff = turn_rate * track_width / 2.f;

    motor_R.setSpeed( speed + diff );
    motor_L.setSpeed( speed - diff );

    // because i am doin it the dirty way
    hey_emulator_speed = speed;
    hey_emulator_rate = turn_rate;
}

void NGC::setControlOutput_radius( float speed, float turn_radius )
{
    if( turn_radius == 0 )
    {
        motor_R.setSpeed( speed );
        motor_L.setSpeed( speed );
        return;
    }

    float track_width = m_vehicle->track;   // metre
    
    // TODO if radius is smaller than the vehicles width/2 there must be reverse track movement

    float abs_radi = fabs(turn_radius);

    float outer_radius = abs_radi + track_width/2.f;
    float inner_radius = abs_radi - track_width/2.f;

    float outer_coef = outer_radius / abs_radi;
    float inner_coef = inner_radius / abs_radi;

    if( turn_radius > 0 )
    {
        motor_R.setSpeed( outer_coef * speed );
        motor_L.setSpeed( inner_coef * speed );
    }else{
        motor_R.setSpeed( inner_coef * speed );
        motor_L.setSpeed( outer_coef * speed );
    }

    // because i am doin it the dirty way
    hey_emulator_speed = speed;
    hey_emulator_rate = -fabs(speed)/turn_radius;
}

void NGC::processPacket( CommsPacket &packet )
{
    // null packet catch
    if( packet.isNull() )
    {
        std::cerr<<"packet was null\n";
        return;
    }

    if( m_command_packet.packet_type != CommsPacket::ngc_command )
        return;

    directCommand(
        packet.getManualSpeed(),
        packet.getManualSteer() );

    // DEBUG
    std::cout<<"NGC: packet type: "<< (int)packet.packet_type<<" "
        <<"data: \n";
    for( std::uint8_t d : packet.data )
        std::cout<< (int)d << " ";
    std::cout<<"\n";
    // DEBUG END
}

