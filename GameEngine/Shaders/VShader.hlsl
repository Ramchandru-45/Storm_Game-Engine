// Input structure for the vertex shader
struct VS_INPUT
{
    float3 position : POSITION; // Vertex position in object space
    float3 color : COLOR; // Vertex color
};

// Output structure for the vertex shader, passed to the pixel shader
struct VS_OUTPUT
{
    float4 position : SV_POSITION; // Vertex position in clip space
    float3 color : COLOR; // Interpolated color
};

// Main function of the vertex shader
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // Transform position to clip space (assuming no world/view/projection matrices for simplicity)
    // In a real application, you would apply transformation matrices here.
    output.position = float4(input.position, 1.0f);
    
    // Pass the color directly to the pixel shader for interpolation
    output.color = input.color;
    
    return output;
}