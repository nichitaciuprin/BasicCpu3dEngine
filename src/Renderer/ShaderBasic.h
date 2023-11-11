#pragma once

class ShaderBasic
{
public:
    ShaderBasic()
    {
        CompileShadersAndInputs();
        CreateConstantBuffer();
        CreateRasterizerState();
        CreateDepthStencilState();
    }

    void Set()
    {
        auto deviceContext = DeviceRecources::GetInstance()->GetDeviceContext();
        deviceContext->RSSetState(rasterizerState);
        deviceContext->OMSetDepthStencilState(depthStencilState, 0);
        deviceContext->IASetInputLayout(inputLayout);
        deviceContext->VSSetShader(vertexShader, nullptr, 0);
        deviceContext->PSSetShader(pixelShader, nullptr, 0);
        deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
    }
    void UpdateConstantBuffer(Matrix modelViewProj)
    {
        auto deviceContext = DeviceRecources::GetInstance()->GetDeviceContext();

        // HLSL will transopose any matrix in constant buffer
        modelViewProj = MatrixTranspose(modelViewProj);

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;
        deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        Constants* constants = (Constants*)(mappedSubresource.pData);
        constants->modelViewProj = modelViewProj;
        deviceContext->Unmap(constantBuffer, 0);
    }

private:
    ID3D11InputLayout*        inputLayout;
    ID3D11VertexShader*       vertexShader;
    ID3D11PixelShader*        pixelShader;
    ID3D11Buffer*             constantBuffer;
    ID3D11RasterizerState*    rasterizerState;
    ID3D11DepthStencilState*  depthStencilState;

    void CompileShadersAndInputs()
    {
        auto device = DeviceRecources::GetInstance()->GetDevice();

        UINT shaderCompileFlags = 0;

        // Compiling with this flag allows debugging shaders with Visual Studio
        #if defined(DEBUG_BUILD)
        shaderCompileFlags |= D3DCOMPILE_DEBUG;
        #endif

        ID3DBlob* vsBlob;
        ID3DBlob* psBlob;
        ID3DBlob* shaderCompileErrorsBlob;

        auto shaderFile = GetExeDirectory() + L"\\shaders.hlsl";
        assert(FileExists(shaderFile));

        auto fileName = (LPCWSTR)shaderFile.c_str();

        {
            HRESULT hResult = D3DCompileFromFile(fileName, nullptr, nullptr, "vs_main", "vs_5_0", shaderCompileFlags, 0, &vsBlob, &shaderCompileErrorsBlob);
            if (FAILED(hResult))
            {
                const char* errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
                shaderCompileErrorsBlob->Release();
                ShowMessageBox(errorString);
                assert(false);
            }

            hResult = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
            assert(SUCCEEDED(hResult));
        }

        {
            HRESULT hResult = D3DCompileFromFile(fileName, nullptr, nullptr, "ps_main", "ps_5_0", shaderCompileFlags, 0, &psBlob, &shaderCompileErrorsBlob);
            if (FAILED(hResult))
            {
                const char* errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
                shaderCompileErrorsBlob->Release();
                ShowMessageBox(errorString);
                assert(false);
            }

            hResult = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
            assert(SUCCEEDED(hResult));
        }

        {
            D3D11_INPUT_ELEMENT_DESC inputElementDescArray[] =
            {
                { "SV_POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
            };

            HRESULT hResult = device->CreateInputLayout(inputElementDescArray, ARRAYSIZE(inputElementDescArray), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
            assert(SUCCEEDED(hResult));
        }

        vsBlob->Release();
        psBlob->Release();
    }
    void CreateConstantBuffer()
    {
        auto device = DeviceRecources::GetInstance()->GetDevice();

        D3D11_BUFFER_DESC constantBufferDesc = {};
        constantBufferDesc.ByteWidth      = (sizeof(Constants) + 16) & 0xfffffff0; // Must be multiple of 16
        constantBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
        constantBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hResult = device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
        assert(SUCCEEDED(hResult));
    }
    void CreateRasterizerState()
    {
        auto device = DeviceRecources::GetInstance()->GetDevice();

        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = FALSE;
        rasterizerDesc.DepthClipEnable = TRUE;
        device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    }
    void CreateDepthStencilState()
    {
        auto device = DeviceRecources::GetInstance()->GetDevice();

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable    = TRUE;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;
        device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
    }

    struct Constants
    {
        Matrix modelViewProj;
    };
};