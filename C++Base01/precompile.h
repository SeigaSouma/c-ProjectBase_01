//=============================================================================
// 
//  �v���R���p�C���w�b�_�[ [precompile.h]
//  Author : ���n�Ή끕���搶
// 
//=============================================================================
#ifndef _PRECOMPIRE_H_
#define _PRECOMPIRE_H_	// ��d�C���N���[�h�h�~

#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <DirectXMath.h>
#include <D3DX9Math.h>

// C++��std�p
#include <vector>
#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iterator>
#include <codecvt>

#include <d3dx9.h>						// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���p�}�N��
#include <dinput.h>						// ���͏����ɕK�v
#include <xaudio2.h>					// �T�E���h�����ɕK�v
#include <dsound.h>						// �T�E���h���������ɕK�v
#include <Xinput.h>

// �}�C���C�u����
#include "mylib.h"

// �G�t�F�N�V�A
#ifndef _DEBUG
#pragma comment(lib, "Effekseer.lib")
#else
#pragma comment(lib, "Effekseerd.lib")
#endif
#include <Effekseer.h>

#ifndef _DEBUG
#pragma comment(lib, "EffekseerRendererDX9.lib")
#else
#pragma comment(lib, "EffekseerRendererDX9d.lib")
#endif
#include <EffekseerRendererDX9.h>


// ���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// ���������[�N���o�ɕK�v

// crtdbg.h�̃o�O�΍��p�}�N��
#if _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else

#define DEBUG_NEW new
#endif

#endif	// _PRECOMPIRE_H_