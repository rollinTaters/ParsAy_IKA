
#include "raylib.h"
#include "raymath.h"

namespace GUI
{

    Vector3 taters2raylib( Vector3 inp )
    { return Vector3RotateByAxisAngle( inp, (Vector3){1,0,0}, 3*PI/2.f ); }

    Vector3 taters2raylib( float x, float y, float z )
    { return Vector3RotateByAxisAngle( (Vector3){x,y,z}, (Vector3){1,0,0}, 3*PI/2.f ); }

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

    void initGUI()
    {
        // create a window for gui rendering
        InitWindow( 1000,800, "NGC DEBUG GUI" );

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
        float wheelbase = 1.3f;
        float track = 1.0f;
        float wheel_dia = 0.5f;
        float wheel_width = 0.25f;
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


};
