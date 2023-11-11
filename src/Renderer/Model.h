#pragma once

class Model
{
public:
    Model(const vector<float>& vertexData, const vector<uint16_t>& indexData)
    {
        CreateVertexBuffer(vertexData);
        CreateIndexBuffer(indexData);
    }
    void Draw()
    {
        UINT stride = sizeof(Vector3);
        UINT offset = 0;
        auto deviceRecources = DeviceRecources::GetInstance();
        auto deviceContext = deviceRecources->GetDeviceContext();
        deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->DrawIndexed(indexCount, 0, 0);
    }

private:
    UINT           indexCount;
    ID3D11Buffer*  vertexBuffer;
    ID3D11Buffer*  indexBuffer;

    void CreateVertexBuffer(const vector<float>& vertexData)
    {
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = (UINT)vertexData.size() * sizeof(float);
        vertexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
        vertexSubresourceData.pSysMem = vertexData.data();

        auto device = DeviceRecources::GetInstance()->GetDevice();

        HRESULT hResult = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
        assert(SUCCEEDED(hResult));
    }
    void CreateIndexBuffer(const vector<uint16_t>& indexData)
    {
        indexCount = (UINT)indexData.size();

        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = (UINT)indexData.size() * sizeof(uint16_t);
        indexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
        indexSubresourceData.pSysMem = indexData.data();

        auto device = DeviceRecources::GetInstance()->GetDevice();

        HRESULT hResult = device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
        assert(SUCCEEDED(hResult));
    }
};

class ModelCube : public Model
{
public:
    ModelCube() : Model(VertexData, IndexData) {}
private:
    static const vector<float> VertexData;
    static const vector<uint16_t> IndexData;
};
const vector<float> ModelCube::VertexData =
{
   -0.5f,-0.5f,-0.5f,
   -0.5f,-0.5f, 0.5f,
   -0.5f, 0.5f,-0.5f,
   -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f
};
const vector<uint16_t> ModelCube::IndexData =
{
    0, 6, 4,
    0, 2, 6,
    0, 3, 2,
    0, 1, 3,
    2, 7, 6,
    2, 3, 7,
    4, 6, 7,
    4, 7, 5,
    0, 4, 5,
    0, 5, 1,
    1, 5, 7,
    1, 7, 3
};

class ModelBird : public Model
{
public:
    ModelBird() : Model(VertexData, IndexData) {}
private:
    static const vector<float> VertexData;
    static const vector<uint16_t> IndexData;
};
const vector<float> ModelBird::VertexData =
{
    0.00f,  0.25f,  1.00f,
    0.00f, -0.50f,  0.00f,
    0.00f,  0.00f,  2.00f,
    2.00f,  0.00f, -2.00f,
   -2.00f,  0.00f, -2.00f,
};
const vector<uint16_t> ModelBird::IndexData =
{
    0, 2, 3,
    0, 4, 2,
    2, 4, 1,
    2, 1, 3,
    0, 3, 1,
    0, 1, 4
};

class ModelOverlapingTriangles : public Model
{
public:
    ModelOverlapingTriangles() : Model(VertexData, IndexData) {}
private:
    static const vector<float> VertexData;
    static const vector<uint16_t> IndexData;
};
const vector<float> ModelOverlapingTriangles::VertexData =
{
    2, 3, 1,
    3, 1, 1,
   -3, 1, 0,
   -3, 1, 1,
   -2, 3, 1,
    1,-3, 0,
    1,-3, 1,
   -1,-3, 1,
    2, 3, 0
};
const vector<uint16_t> ModelOverlapingTriangles::IndexData =
{
    0, 1, 2,
    3, 4, 5,
    6, 7, 8
};