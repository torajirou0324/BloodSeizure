#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_4.h>

// Linker
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class SceneManager;

class App
{
public:
	App() = delete;
	App(const TCHAR* appName);
	~App();

	void GameLoop();
private:
	void StartApp(const TCHAR* appName);
	void InitWindow(const TCHAR* appName);

	// D3D12 methods
	bool Init3D();
	void Term3D();
	void Render();
	void WaitGpu();
	void Present(UINT32 interval);

	SceneManager* m_sceneManager;		// �V�[���}�l�[�W���[�N���X�̃|�C���^
	
	static const UINT FrameCount = 2;	// �t���[���o�b�t�@

	HINSTANCE		m_hInst;				// �C���X�^���X�n���h��
	HWND			m_hWnd;					// �E�B���h�E�n���h��
	const UINT		WINDOW_WIDTH = 1280;	// �E�B���h�E�̉���
	const UINT		WINDOW_HEIGHT = 720;	// �E�B���h�E�̏c��
	const TCHAR*	ClassName;				// �E�B���h�E�N���X��

	ID3D12Device*				m_pDevice;					// �ǉ��f�o�C�X
	ID3D12CommandQueue*			m_pQueue;					// �R�}���h�L���[
	IDXGISwapChain3*			m_pSwapChain;				// �X���b�v�`�F�C��
	ID3D12Resource*				m_pColorBuffer[FrameCount];	// �J���[�o�b�t�@
	ID3D12CommandAllocator*		m_pCmdAllocator[FrameCount];// �R�}���h�A���[�P�[�^
	ID3D12GraphicsCommandList*	m_pCmdList;					// �R�}���h���X�g
	ID3D12DescriptorHeap*		m_pHeapRTV;					// �f�B�X�N���v�^�q�[�v�i�����_�[�^�[�Q�b�g�r���[�j
	ID3D12Fence*				m_pFence;					// �t�F���X
	HANDLE						m_FenceEvent;				// �t�F���X�C�x���g
	UINT16						m_FenceCounter[FrameCount];	// �t�F���X�J�E���^�[
	UINT32						m_FrameIndex;				// �t���[���ԍ�
	D3D12_CPU_DESCRIPTOR_HANDLE m_HandleRTV[FrameCount];	// CPU�f�B�X�N���v�^�q�[�v�i�����_�[�^�[�Q�b�g�r���[�j
};
