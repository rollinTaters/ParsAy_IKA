
#include "raylib.h"
#include "raymath.h"

// a slight nod to external objects
extern NGC ngc_system;
extern Vehicle simulated_vehicle;
extern Env_Emulator env_emulator;

namespace GUI
{

    Vector3 taters2raylib( Vector3 inp )
    { return Vector3RotateByAxisAngle( inp, (Vector3){1,0,0}, 3*PI/2.f ); }

    Vector3 taters2raylib( float x, float y, float z )
    { return Vector3RotateByAxisAngle( (Vector3){x,y,z}, (Vector3){1,0,0}, 3*PI/2.f ); }

    Vector3 taters2raylib( Point p)
    { return Vector3RotateByAxisAngle( (Vector3){p.x,p.y,p.z}, (Vector3){1,0,0}, 3*PI/2.f ); }

    Vector3 raylib2taters( Vector3 inp )
    { return Vector3RotateByAxisAngle( inp, (Vector3){1,0,0}, PI/2.f ); }

    // NOTE: These variables will live in main.cpp
    Camera3D camera;
    Font font;
    Texture2D texture_xp;
    Texture2D texture_xn;
    Texture2D texture_yp;
    Texture2D texture_yn;
    Texture2D texture_zp;
    Texture2D texture_zn;

    // crosshair variables
    Vector3 crosshair;
    float ch_gap;   // empty distance in the center
    float ch_len;   // line length
    Color ch_color; // color of crosshair lines

    std::vector<Point> ngc_wps;
    double last_wp_update;
    double wp_update_rate;

    void initGUI()
    {
        // create a window for gui rendering
        InitWindow( 1000,800, "NGC DEBUG GUI" );

        crosshair = {0,0,0};
        ch_gap = 0.05;
        ch_len = 0.15;
        ch_color = LIME;

        // camera setup
        camera = {0};
        camera.position = (Vector3){ 0.f, 10.f, 10.f };
        camera.target = (Vector3){ 0.f, 0.f, 0.f };
        camera.up = (Vector3){ 0.f, 1.f, 0.f };
        camera.fovy = 45.f;
        camera.projection = CAMERA_PERSPECTIVE;

        font = LoadFont( "./gfx/PixeloidSans.ttf" );
        texture_xp = LoadTextureFromImage( ImageTextEx( font, "X+", 110, 5, BLACK ) );
        texture_xn = LoadTextureFromImage( ImageTextEx( font, "X-", 110, 5, BLACK ) );
        texture_yp = LoadTextureFromImage( ImageTextEx( font, "Y+", 110, 5, BLACK ) );
        texture_yn = LoadTextureFromImage( ImageTextEx( font, "Y-", 110, 5, BLACK ) );
        texture_zp = LoadTextureFromImage( ImageTextEx( font, "Z+", 110, 5, BLACK ) );
        texture_zn = LoadTextureFromImage( ImageTextEx( font, "Z-", 110, 5, BLACK ) );

        SetTargetFPS(60);

        ngc_wps = ngc_system.getWPs();
        last_wp_update = GetTime();
        wp_update_rate = 1; // seconds
    }

    void deInitGUI()
    {
        UnloadTexture( texture_xp );
        UnloadTexture( texture_xn );
        UnloadTexture( texture_yp );
        UnloadTexture( texture_yn );
        UnloadTexture( texture_zp );
        UnloadTexture( texture_zn );
        // de-initialization of window and opengl context
        CloseWindow();
    }

    void checkUserInput()
    {
        // moving camera around
        if( IsKeyDown( KEY_H ) )
            camera.position = Vector3RotateByAxisAngle( camera.position, camera.up, -2*DEG2RAD );
        if( IsKeyDown( KEY_L ) )
            camera.position = Vector3RotateByAxisAngle( camera.position, camera.up,  2*DEG2RAD );
        if( IsKeyDown( KEY_J ) )
            camera.position = Vector3RotateByAxisAngle( camera.position, Vector3CrossProduct(camera.position-camera.target, camera.up), -2*DEG2RAD );
        if( IsKeyDown( KEY_K ) )
            camera.position = Vector3RotateByAxisAngle( camera.position, Vector3CrossProduct(camera.position-camera.target, camera.up),  2*DEG2RAD );
        if( IsKeyDown( KEY_N ) )
            camera.position += Vector3Normalize((camera.position-camera.target)) *  0.2f;
        if( IsKeyDown( KEY_M ) )
            camera.position += Vector3Normalize((camera.position-camera.target)) * -0.2f;

        constexpr float ch_speed = 0.1;
        // moving crosshair
        if( IsKeyDown( KEY_W ) )
            crosshair += { 0, +ch_speed, 0};
        if( IsKeyDown( KEY_S ) )
            crosshair += { 0, -ch_speed, 0};
        if( IsKeyDown( KEY_A ) )
            crosshair += { -ch_speed, 0, 0};
        if( IsKeyDown( KEY_D ) )
            crosshair += { +ch_speed, 0, 0};
        if( IsKeyDown( KEY_R ) )
            crosshair += { 0, 0, +ch_speed};
        if( IsKeyDown( KEY_F ) )
            crosshair += { 0, 0, -ch_speed};

        // ngc commands
        if( IsKeyPressed( KEY_X ) )
            ::ngc_system.executeWPs();
        if( IsKeyPressed( KEY_C ) )
        {
            Point p(crosshair.x, crosshair.y, crosshair.z);
            Point p_veh = simulated_vehicle.getPos();
            ::ngc_system.addWP( p+p_veh );

            std::cout<<"p_veh: "<<p_veh.x<<"x "<<p_veh.y<<"y "<<p_veh.z<<"z\n";    // DEBUG
            std::cout<<"p_cro: "<<p.x<<"x "<<p.y<<"y "<<p.z<<"z\n";    // DEBUG
        }


    }

    void drawAxisBillboards()
    {
        // camera, texture, position, scale, color
        DrawBillboard( camera, texture_xp, taters2raylib((Vector3){ 5, 0, 0}), 0.5f, WHITE );
        DrawBillboard( camera, texture_xn, taters2raylib((Vector3){-5, 0, 0}), 0.5f, WHITE );
        DrawBillboard( camera, texture_yp, taters2raylib((Vector3){ 0, 5, 0}), 0.5f, WHITE );
        DrawBillboard( camera, texture_yn, taters2raylib((Vector3){ 0,-5, 0}), 0.5f, WHITE );
        DrawBillboard( camera, texture_zp, taters2raylib((Vector3){ 0, 0, 5}), 0.5f, WHITE );
        DrawBillboard( camera, texture_zn, taters2raylib((Vector3){ 0, 0,-5}), 0.5f, WHITE );
        // slices, spacing
        DrawGrid(10, 2);

    }

    void drawVehicle()
    {
        float wheelbase = simulated_vehicle.wheelbase;
        float track = simulated_vehicle.track;
        float wheel_dia = simulated_vehicle.wheel_dia;
        float wheel_width = simulated_vehicle.wheel_width;
        // front right wheel
        DrawCylinderEx( taters2raylib( track/2 +wheel_width/2, wheelbase/2, 0 ),
                        taters2raylib( track/2 -wheel_width/2, wheelbase/2, 0 ),
                        wheel_dia/2, wheel_dia/2, 20, DARKGRAY );
        // front left wheel
        DrawCylinderEx( taters2raylib( -track/2 +wheel_width/2, wheelbase/2, 0 ),
                        taters2raylib( -track/2 -wheel_width/2, wheelbase/2, 0 ),
                        wheel_dia/2, wheel_dia/2, 20, DARKGRAY );
        // rear right wheel
        DrawCylinderEx( taters2raylib( track/2 +wheel_width/2, -wheelbase/2, 0 ),
                        taters2raylib( track/2 -wheel_width/2, -wheelbase/2, 0 ),
                        wheel_dia/2, wheel_dia/2, 20, DARKGRAY );
        // rear left wheel
        DrawCylinderEx( taters2raylib( -track/2 +wheel_width/2, -wheelbase/2, 0 ),
                        taters2raylib( -track/2 -wheel_width/2, -wheelbase/2, 0 ),
                        wheel_dia/2, wheel_dia/2, 20, DARKGRAY );
        // body
        DrawCubeV( taters2raylib((Vector3){0,0,0.20f}), taters2raylib((Vector3){1.15f, 1.65f, 0.50f}), GREEN ); 
    }

    void drawCrosshair()
    {
        // x+
        DrawLine3D( taters2raylib(crosshair+(Vector3){ch_gap,0,0}),
                    taters2raylib(crosshair+(Vector3){ch_gap+ch_len,0,0}),
                    ch_color );
        // x-
        DrawLine3D( taters2raylib(crosshair+(Vector3){-ch_gap,0,0}),
                    taters2raylib(crosshair+(Vector3){-ch_gap-ch_len,0,0}),
                    ch_color );
        // y+
        DrawLine3D( taters2raylib(crosshair+(Vector3){0,ch_gap,0}),
                    taters2raylib(crosshair+(Vector3){0,ch_gap+ch_len,0}),
                    ch_color );
        // y-
        DrawLine3D( taters2raylib(crosshair+(Vector3){0,-ch_gap,0}),
                    taters2raylib(crosshair+(Vector3){0,-ch_gap-ch_len,0}),
                    ch_color );
        // z+
        DrawLine3D( taters2raylib(crosshair+(Vector3){0,0,ch_gap}),
                    taters2raylib(crosshair+(Vector3){0,0,ch_gap+ch_len}),
                    ch_color );
        // z-
        DrawLine3D( taters2raylib(crosshair+(Vector3){0,0,-ch_gap}),
                    taters2raylib(crosshair+(Vector3){0,0,-ch_gap-ch_len}),
                    ch_color );
        
        // drop line
        if( crosshair.z > (ch_gap+ch_len + 0.05) )
            DrawLine3D( taters2raylib(crosshair+(Vector3){0,0,-crosshair.z}),
                        taters2raylib(crosshair+(Vector3){0,0,-ch_gap-ch_len}),
                        ch_color );
        if( crosshair.z < -(ch_gap+ch_len + 0.05) )
            DrawLine3D( taters2raylib(crosshair+(Vector3){0,0,-crosshair.z}),
                        taters2raylib(crosshair+(Vector3){0,0,ch_gap+ch_len}),
                        ch_color );


    }

    void drawOverlay()
    {
        // font, text, position2, font size, spacing, color
        DrawTextEx(
                font,
                "H,J,K,L: camera movement\n"
                "N,M: zoom\n"
                "W,A,S,D,R,F: crosshair move\n"
                "X: execute waypoints\n"
                "C: create waypoint",
                (Vector2){10,600}, 20, 2, DARKGRAY );
        DrawTextEx(
                font,
                TextFormat("crosshair: %3.2fx %3.2fy %3.2fz", crosshair.x, crosshair.y, crosshair.z),
                (Vector2){10,30}, 20, 2, DARKGRAY );
        DrawTextEx(
                font,
                TextFormat("num WPs: %d", ngc_wps.size()),
                (Vector2){10,50}, 20, 2, DARKGRAY );
        for( int i = 0; i < (int)ngc_wps.size(); i++ )
        {
            Point p = ngc_wps[i];
            DrawTextEx( font, TextFormat("wp%d: %3.1fx %3.1fy %3.1fz", i, p.x, p.y, p.z),
                        (Vector2){160, 50+(16.f*i)}, 16, 1, DARKGRAY );
        }
    }

    void drawWPs()
    {
        if( last_wp_update + wp_update_rate < GetTime() )
            ngc_wps = ngc_system.getWPs();

        Point p_veh = simulated_vehicle.getPos();

        int wp_num = 0;
        for( Point p : ngc_wps )
        {
            //DrawPoint3D( taters2raylib(p), ORANGE );
            Vector2 pos2d = GetWorldToScreen( taters2raylib(p-p_veh), camera );
            DrawCircle( pos2d.x, pos2d.y, 5.0, ORANGE );
            DrawTextEx( font, TextFormat("%d",wp_num),
                        pos2d, 20, 1, ORANGE );
            wp_num++;
        }
    }

    void drawLIDAR()
    {
        std::vector<Point> pts = ngc_system.getImObPoints();
        //std::cout<<"DEBUG: ImObPoints count: "<< pts.size()<<"\n";  // DEBUG
        for( Point &p : pts )
        {
            DrawSphere( taters2raylib(p), 0.1, RED );
            std::cout<<"ImObP: "<<p.x<<"x "<<p.y<<"y "<<p.z<<"z\n"; // DEBUG
        }
    }


};
