// �W���w�b�_�̃C���N���[�h
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>

// �萔�̒�`
#define GEN_MAX			1000	// �����㐔
#define POP_SIZE		1000	// �̌Q�̃T�C�Y
#define ELITE			1		// �G���[�g�ۑ��헪�Ŏc���̂̐�
#define MUTATE_PROB		0.01	// �ˑR�ψيm��  0.01
#define N				64		// �W���̗v�f�ƂȂ�ő吔�̕����l
#define TOURNAMENT_SIZE	30		// �g�[�i�����g�T�C�Y

// 0�ȏ�1�ȉ��̎�������
#define RAND_01 ((double)rand() / RAND_MAX)

class Individual
{
public:
	Individual();
	~Individual();
	void evaluate();									// �K���x���Z�o����
	void crossover1(Individual *p1, Individual *p2);	// �����ɂ��q�ɂ���
	void crossover2(Individual *p1, Individual *p2);	// �����ɂ��q�ɂ���
	void crossoveru(Individual *p1, Individual *p2);	// �����ɂ��q�ɂ���
	void mutate();									// �ˑR�ψق��N����

	int chrom[N];	// ���F��, �t���C�h���ɂ�������F�̂�1�`N�̐������ǂ̏W���ɑ����Ă��邩�����Ă���
	double fitness;	// �K���x
};
