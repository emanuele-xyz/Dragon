cbuffer CBCamera : register(b0)
{
    struct
    {
        matrix view;
        matrix projection;
    } cb_camera;
};
