// Input structure for the pixel shader (matches VS_OUTPUT)
struct PS_INPUT
{
    float4 position : SV_POSITION; // Interpolated position (not typically used for color)
    float3 color : COLOR; // Interpolated color from the vertex shader
};

// Main function of the pixel shader
float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    // Return the interpolated color as the final pixel color
    return float4(input.color, 1.0f); // Add alpha component (1.0 for opaque)
}