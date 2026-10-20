
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 5) in ivec4 aBoneIDs;
layout(location = 6) in vec4 aBoneWeights;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform bool IsSkinned;
uniform samplerBuffer BoneBuffer;

out vec2 TexCoord;

mat4 GetBoneMatrix(int boneID)
{
    int baseIndex = boneID * 4;

    return mat4(
        texelFetch(BoneBuffer, baseIndex + 0),
        texelFetch(BoneBuffer, baseIndex + 1),
        texelFetch(BoneBuffer, baseIndex + 2),
        texelFetch(BoneBuffer, baseIndex + 3)
    );
}

void main()
{
    vec4 localPosition = vec4(aPos, 1.0);

    if (IsSkinned)
    {
        mat4 skinMatrix = mat4(0.0);
        float totalWeight = 0.0;

        for (int i = 0; i < 4; i++)
        {
            int boneID = aBoneIDs[i];
            float weight = aBoneWeights[i];

            if (weight > 0.0)
            {
                skinMatrix += GetBoneMatrix(boneID) * weight;
                totalWeight += weight;
            }
        }

        if (totalWeight > 0.0)
        {
            localPosition = skinMatrix * localPosition;
        }
    }

    gl_Position = Projection * View * Model * localPosition;
    TexCoord = aTexCoord;
}
