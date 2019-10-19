// d3dshaderdisasm_cli.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "D3dcompiler.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include <stdio.h>
#include <D3Dcompiler.h>

int main(int argc, char* argv[])
{
	int curArg = 1;
	int flags = 0;

	if(argc < 3)
		return -1;
	if(argc > 3) {
		for(int i = 0; i < argc - 3; i++) {
			const char *param = argv[curArg++];
			if(strcmp(param, "-f") == 0) {
				flags |= D3D_DISASM_ENABLE_COLOR_CODE;
			}
		}
	}

	const char *src = argv[curArg++];
	const char *dst = argv[curArg++];

	FILE *fp = fopen(src, "rb");

	fseek(fp, 0, SEEK_END);
	int fileSize;
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	BYTE *shaderBinData = new BYTE[fileSize];
	fread(shaderBinData, fileSize, 1, fp);
	fclose(fp);

	ID3DBlob *blob;

	HRESULT hr = D3DDisassemble(shaderBinData, fileSize, flags, NULL, &blob);

	FILE * of = fopen(dst, "w");
	fwrite(blob->GetBufferPointer(), blob->GetBufferSize()-1, 1, of); // blob->GetBufferSize() - trailing 0
	fclose(of);

	return hr;
}

