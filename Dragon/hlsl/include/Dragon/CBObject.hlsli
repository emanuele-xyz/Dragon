cbuffer CBObject : register(b1)
{
    struct
    {
        matrix model;
        matrix normal;
    } cb_object;
};
