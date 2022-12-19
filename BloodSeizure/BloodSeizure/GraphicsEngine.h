#pragma once
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <vector>
#include "ComPtr.h"
//#include "RenderContext.h"

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
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    GraphicsEngine(){}

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
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

    GraphicsEngine(const GraphicsEngine&) = delete;             // �R�s�[�R���X�g���N�^��delete�w��
    GraphicsEngine& operator=(const GraphicsEngine&) = delete;  // �R�s�[������Z�q��delete�w��
    GraphicsEngine(GraphicsEngine&&) = delete;                  // ���[�u�R���X�g���N�^��delete�w��
    GraphicsEngine& operator=(GraphicsEngine&&) = delete;       // ���[�u������Z�q��delete�w��

    /// <summary>
    /// D3D�f�o�C�X
    /// </summary>
    /// <returns></returns>
    ID3D12Device* GetD3DDevice()
    {
        return m_pDevice.Get();
    }

    /// <summary>
    /// �o�b�N�o�b�t�@�̔ԍ����擾
    /// </summary>
    /// <returns></returns>
    UINT GetBackBufferIndex() const
    {
        return m_frameIndex;
    }

    /// <summary>
    /// �����_�����O�R���e�L�X�g���擾
    /// </summary>
    /// <returns></returns>
    //RenderContext& GetRenderContext()
    //{
    //    return m_renderContext;
    //}

    /// <summary>
    /// �R�}���h���X�g���擾
    /// </summary>
    /// <returns></returns>
    ID3D12GraphicsCommandList* GetCommandList() const
    {
        return m_pCommandList.Get();
    }

    /// <summary>
    /// �R�}���h�L���[���擾
    /// </summary>
    /// <returns></returns>
    ID3D12CommandQueue* GetCommandQueue() const
    {
        return m_pCommandQueue.Get();
    }

private:
    /// <summary>
    /// ��ʂɕ\�����A���̃t���[���̏������s��
    /// </summary>
    /// <param name="interval"></param>
    void Present(uint32_t interval);

    /// <summary>
    /// �`��̊����҂�
    /// </summary>
    void WaitDraw();

public:
    /// <summary>
    /// �t���[���o�b�t�@�̐�
    /// </summary>
    static const int FRAME_BUFFER_COUNT = 2;

private:
    /// <summary>Direct3D�f�o�C�X</summary>
    ComPtr<ID3D12Device> m_pDevice = nullptr;
    /// <summary>�R�}���h�L���[</summary>
    ComPtr<ID3D12CommandQueue> m_pCommandQueue = nullptr;
    /// <summary>�X���b�v�`�F�C��</summary>
    ComPtr<IDXGISwapChain3> m_pSwapChain = nullptr;
    /// <summary>�R�}���h�A���P�[�^</summary>
    ComPtr<ID3D12CommandAllocator> m_pCommandAllocator[FRAME_BUFFER_COUNT] = { nullptr };
    /// <summary>�R�}���h���X�g</summary>
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList = nullptr;
    /// <summary>�p�C�v���C���X�e�[�g</summary>
    ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;

    /// <summary>���݂̃o�b�N�o�b�t�@�̔ԍ�</summary>
    int m_currentBackBufferIndex = 0;
    /// <summary>�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_rtvDescriptorSize = 0;
    /// <summary>�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_dsvDescriptorSize = 0;
    /// <summary>CBV_SRV_UAV�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_cbvSrvUavDescriptorSize = 0;
    /// <summary>�T���v���[�̃f�B�X�N���v�^�̃T�C�Y</summary>
    UINT m_samplerDescriptorSize = 0;

    /// <summary>�����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v</summary>
    ID3D12DescriptorHeap* m_pRtvHeap = nullptr;
    /// <summary>�t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g</summary>
    ID3D12Resource* m_pRenderTargets[FRAME_BUFFER_COUNT] = { nullptr };
    /// <summary>�[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v</summary>
    ID3D12DescriptorHeap* m_pDsvHeap = nullptr;
    /// <summary>�[�x�X�e���V���o�b�t�@</summary>
    ID3D12Resource* m_pDepthStencilBuffer = nullptr;
    /// <summary>�r���[�|�[�g</summary>
    D3D12_VIEWPORT m_viewport;
    /// <summary>�V�U�����O��`</summary>
    D3D12_RECT m_scissorRect;
    /// <summary>�����_�����O�R���e�L�X�g</summary>
    //RenderContext m_renderContext;

    /// <summary>���ݏ������ݒ��̃t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�̃n���h��</summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;
    /// <summary>���ݏ������ݒ��̃t���[���o�b�t�@�̐[�x�X�e���V���r���[�̃n���h��</summary>
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;

    /// <summary>
    /// GPU�Ƃ̓����Ŏg�p����ϐ�
    /// </summary>
    UINT m_frameIndex = 0;
    HANDLE m_FenceEvent = nullptr;
    ID3D12Fence* m_pFence = nullptr;
    UINT64 m_fenceValue = 0;
    UINT m_frameBufferWidth = 0;
    UINT m_frameBufferHeight = 0;
};

/// <summary>�O���t�B�b�N�X�G���W���̃A�h���X���i�[����O���[�o���ϐ�</summary>
extern GraphicsEngine* g_graphicsEngine;