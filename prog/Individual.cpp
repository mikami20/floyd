#include "Individual.h"  //Individual = �́C�l

// �R���X�g���N�^
Individual::Individual()
{
	int i;

	for(i = 0; i < N; i++) {
		chrom[i] = rand() % 2;  //�̂̊e���F�̂�0��1�̗���������C1�͏W��A�ɑ�����E0�͏W��B�ɑ�����
	}
	fitness = 0.0;  //�K���x��0�ŏ������C�K���x�͏W���̍��ŕ\��
}

// �f�X�g���N�^
Individual::~Individual()
{
}

// �K���x���Z�o����
void Individual::evaluate()
{
	int i;

	fitness = 0.0;
	for(i = 0; i < N; i++) {  // �W���̗v�f�ƂȂ�ő吔�̕����l, ����̏ꍇ64
		fitness += (chrom[i] * 2 - 1) * sqrt((double)i + 1);  //�K���x�֐��I�I
	}
	fitness = fabs(fitness);  //��Βl���Ƃ�
}

// p1��p2�����_�����ō�����q�ɂ���
// p1: �e��1
// p2: �e��2
void Individual::crossover1(Individual *p1, Individual *p2)
{
	int point, i;

	point = rand() % (N - 1);  //0����N-1�ȉ��̐����̗���
	for(i = 0; i <= point; i++) {
		chrom[i] = p1->chrom[i];  //0����point�܂�p1�̈�`�q��n��
	}
	for(; i < N; i++) {
		chrom[i] = p2->chrom[i];  //point����63�܂�p2�̈�`�q��n��
	}
}

// p1��p2�����_�����ō�����q�ɂ���
// p1: �e��1
// p2: �e��2
void Individual::crossover2(Individual *p1, Individual *p2)
{
	int point1, point2, tmp, i;

	point1 = rand() % (N - 1);
	point2 = (point1 + (rand() % (N - 2) + 1)) % (N - 1);
	if(point1 > point2) {
		tmp = point1;
		point1 = point2;
		point2 = tmp;
	}
	for(i = 0; i <= point1; i++) {
		chrom[i] = p1->chrom[i];
	}
	for(; i <= point2; i++) {
		chrom[i] = p2->chrom[i];
	}
	for(; i < N; i++) {
		chrom[i] = p1->chrom[i];
	}
}

// p1��p2�����l�����ō�����q�ɂ���
// p1: �e��1
// p2: �e��2
void Individual::crossoveru(Individual *p1, Individual *p2)
{
	int i;

	for(i = 0; i < N; i++) {
		if(rand() % 2 == 1) {
			chrom[i] = p1->chrom[i];
		} else {
			chrom[i] = p2->chrom[i];
		}
	}
}

// �ˑR�ψق��N����
void Individual::mutate()
{
	int i;

	for(i = 0; i < N; i++) {  //�S�Ă̐��F�̂ɑ΂��Ĉȉ��̑�������{����
		if(RAND_01 < MUTATE_PROB) {  //0�ȏ�1�ȉ��̎���������0.01�ȉ��ɂȂ�ΐ��F�̂�S�Ĕ��]
			chrom[i] = 1 - chrom[i];
		}
	}
}
