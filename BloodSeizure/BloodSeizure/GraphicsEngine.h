#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "ComPtr.h"

#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3dcompiler.lib" )

/// <summary>
/// DirectX12�̃O���t�B�b�N�G���W��
/// </summary>
class GraphicsEngine
{
public:
    ~GraphicsEngine();

    /// <summary>
    /// DirectX12�̋@�\��������
    /// </summary>
    /// <param name="hwnd">�E�B���h�E�n���h��</param>
    /// <param name="frameBufferWidth">�E�B���h�E��ʂ̉���</param>
    /// <param name="frameBufferHeight">�E�B���h�E��ʂ̏c��</param>
    /// <returns>false���Ԃ��Ă�����쐬�Ɏ��s </returns>
    bool Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

    /// <summary>
    /// �����_�����O�J�n
    /// </summary>
    /// ���t���[���̃����_�����O�J�n���ɌĂԂ���
    void BeginRender();

    /// <summary>
    /// �����_�����O�I��
    /// </summary>
    /// ���t���[���̃����_�����O�I�����ɌĂԂ���
    void EndRender();

    static const int FRAME_BUFFER_COUNT = 2;   // �t���[���o�b�t�@�̐�
private:
    /// <summary>Direct3D�f�o�C�X</summary>
    ID3D12Device* m_d3dDevice = nullptr;
    /// <summary>�R�}���h�L���[</summary>
    ID3D12CommandQueue* m_commandQueue = nullptr;
    /// <summary>�X���b�v�`�F�C��</summary>
    IDXGISwapChain3* m_swapChain = nullptr;
    /// <summary>�R�}���h�A���P�[�^</summary>
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    /// <summary>�R�}���h���X�g</summary>
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    /// <summary>�p�C�v���C���X�e�[�g</summary>
    ID3D12PipelineState* m_pipelineState = nullptr;

    /// <summary>���݂̃o�b�N�o�b�t�@�̔ԍ�</summary>
    int m_currentBackBufferIndex = 0;
    /// <summary>�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_rtvDescriptorSize = 0;
    /// <summary>�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_dsvDescriptorSize = 0;
    /// <summary>CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_cbvSrvDescriptorSize = 0;
    /// <summary>�T���v���[�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_samplerDescriptorSize = 0;

    /// <summary></summary>
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    /// <summary></summary>
    ID3D12Resource* m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };
    /// <summary></summary>
    ID3D12DescriptorHeap* m_dsvHeap = nullptr;
    /// <summary></summary>
    ID3D12Resource* m_depthStencilBuffer = nullptr;
    /// <summary></summary>
    D3D12_VIEWPORT m_viewport;
    /// <summary></summary>
    D3D12_RECT m_scissorRect;

    /// <summary></summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;
    /// <summary></summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;
};

/// <summary>�O���t�B�b�N�X�G���W���̃A�h���X���i�[����O���[�o���ϐ�</summary>
extern GraphicsEngine* g_graphicsEngine;