
// ���_�V�F�[�_�[�̏o�̓f�[�^
struct VSOutput
{
	float4 Position : SV_POSITION;	// �ʒu���W
	float4 Color    : COLOR;		// ���_�J���[
};

struct PSOutput
{
	float4 Color : SV_TARGET0;		// �s�N�Z���J���[
};

PSOutput main(VSOutput input)
{
	PSOutput output = (PSOutput)0;

	output.Color = input.Color;

	return output;
}