#include "Population.h"  //Population = ��W�c�C�̌Q

// �R���X�g���N�^, new�����Ǝ��s�����
Population::Population()  //Population�N���X�̊֐��ł���Population()���`���Ă���
{
	int i;

	ind = new Individual* [POP_SIZE]; //POP SIZE�v�f���������z�񂪏o��������, POP SIZE��individual.h�ɋL�ڂ���Ă���, POP SIZE�͌̌Q�̃T�C�Y, ����1000
	nextInd = new Individual* [POP_SIZE];  //������
	for(i = 0; i < POP_SIZE; i++) {
		ind[i] = new Individual();  //Individual�̃I�u�W�F�N�g���w���悤�ɂ���, ind[]�Ƃ����z��͌̂�\�����F�̂�v�f�Ɏ���
		nextInd[i] = new Individual();  //nextInd�͎��̐�������Ƃ��p�̏ꏊ
	}
	evaluate();
}

// �f�X�g���N�^
Population::~Population()  //�Ō�̂��|��
{
	int i;

	for(i = 0; i < POP_SIZE; i++) {
		delete ind[i];
		delete nextInd[i];
	}
	delete [] ind;
	delete [] nextInd;
}

// ���ׂĂ̌̂�]�����āC�K���x���ɕ��ёւ���
void Population::evaluate()
{
	int i;

	for(i = 0; i < POP_SIZE; i++) {
		ind[i]->evaluate();  //����́CIndividual��evaluate, ind[]�̊e�z��ɑ΂��Ċ֐�evaluate�����s����Ǝw�����o���Ă���
	}
	sort(0, POP_SIZE - 1);  //POP_SIZE�̔z��Ɍ̂����Ă���̂œY������POP_SIZE - 1��
}

// ind[lb]�`ind[ub]���N�C�b�N�\�[�g�ŕ��ёւ���
// lb: ���ёւ��̑Ώۗv�f�̓Y�����̉���
// ub: ���ёւ��̑Ώۗv�f�̓Y�����̏��
void Population::sort(int lb, int ub)
{
	int i, j, k;
	double pivot;  //pivot = ���S(�_), ����̏ꍇ�̓N�C�b�N�\�[�g�̋��E�_�Ƃ��ė��p
	Individual *tmp;  //tmp�̓|�C���^, �A�h���X������, tmp�Ƃ����I�u�W�F�N�g���w�����������o��������

	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = ind[k]->fitness;  //��k�̓K���x��pivot�ɑ��
		i = lb;
		j = ub;
		do {
			while(ind[i]->fitness < pivot) {  //�̂̓K���x��pivot�̓K���x������܂�while���͑���
				i++;
			}
			while(ind[j]->fitness > pivot) {
				j--;
			}
			if(i <= j) {  //�����瑫���Ă���i�ƁC�ォ������Ă���j�����while���𔲂��Ă����̂�i�̕����������Ƃ���pivot�Ƃ̑��Ί֌W�����ւ���
				tmp = ind[i];
				ind[i] = ind[j];
				ind[j] = tmp;
				i++;
				j--;
			}
		} while(i <= j);
		sort(lb, j);
		sort(i, ub);
	}
}

// �����������
void Population::alternate()
{
	int i, j, p1, p2;
	Individual **tmp;  //tmp�̓|�C���^, �A�h���X������, �I�u�W�F�N�g���w������tmp�Ƃ��������o��������
	denom = 0.0;  // ���[���b�g�I���̊m�������߂�Ƃ��̕���

	// ���[���b�g�I���̂��߂̏���
	/*
	for(i = 0; i < POP_SIZE; i++) {
		trFit[i] = (ind[POP_SIZE - 1]->fitness - ind[i]->fitness) / (ind[POP_SIZE - 1]->fitness - ind[0]->fitness);
		denom += trFit[i];
	}
	*/

	// �G���[�g�ۑ��헪�Ŏq�̂����
	for(i = 0; i < ELITE; i++) {  // �G���[�g�ۑ��헪�Ŏc���̂̐�
		for(j = 0; j < N; j++) {
			nextInd[i]->chrom[j] = ind[i]->chrom[j];  //��Ɏ�����ɐ��F�̂��R�s�[���Ă���
		}
	}

	// �e��I������������
	for(; i < POP_SIZE; i++) {  //�Ȃ�i�����������Ă��Ȃ���������Ȃ��C�s��l�ɂȂ�Ȃ��̂��H

		// ���ʂɊ�Â������L���O�I��
		p1 = rankingSelect1();
		p2 = rankingSelect1();
		/*
		// �m���Ɋ�Â������L���O�I��
		p1 = rankingSelect2();
		p2 = rankingSelect2();
		// ���[���b�g�I��
		p1 = rouletteSelect();
		p2 = rouletteSelect();
		// �g�[�i�����g�I��
		p1 = tournamentSelect();
		p2 = tournamentSelect();
		*/

		// ��_����
		nextInd[i]->crossover1(ind[p1], ind[p2]);  //crossover1��Individual�̊֐�
		/*
		// ��_����
		nextInd[i]->crossover2(ind[p1], ind[p2]);
		// ��l����
		nextInd[i]->crossoveru(ind[p1], ind[p2]);
		*/
	}

	// �ˑR�ψق��N�����C�ˑR�ψق̓G���[�g�ɂ͋N�����Ȃ�
	for(i = 1; i < POP_SIZE; i++) {
		nextInd[i]->mutate();
	}

	// �������������ɕύX����
	tmp = ind;
	ind = nextInd;
	nextInd = tmp;

	// �]������
	evaluate();
}

// ���ʂɊ�Â������L���O�I���Őe�̂�1�I������
// �߂�l: �I�������e�̂̓Y����
int Population::rankingSelect1()
{
	int num, denom, r;  //����denom�̗L���͈͂̓u���b�N�L���͈͂ł���C�w�b�_�t�@�C���Œ�`����Ă���denom�Ƃ͕ʕ�

	denom = POP_SIZE * (POP_SIZE + 1) / 2;  //POP_SIZE = �̌Q�̃T�C�Y, �̂̐��C���݂�1000
	r = ((rand() << 16) + (rand() << 1) + (rand() % 2)) % denom + 1;  //0 �ȏ� denom �ȉ��̗���
	for(num = POP_SIZE; 0 < num; num--) {  
		if(r <= num) {
			break;
		}
		r -= num;
	}
	return POP_SIZE - num;  //POP_SIZE - num = �e�̂̓Y����
}

// �m���Ɋ�Â������L���O�I���Őe�̂�1�I������
// �߂�l: �I�������e�̂̓Y����
int Population::rankingSelect2()
{
	int rank, denom;
	double prob, r;

	denom = POP_SIZE * (POP_SIZE + 1) / 2;
	r = RAND_01;
	for(rank = 1; rank < POP_SIZE; rank++) {
		prob = (double)(POP_SIZE - rank + 1) / denom;
		if(r <= prob) {
			break;
		}
		r -= prob;
	}
	return rank - 1;
}

// ���[���b�g�I���Őe�̂�1�I������
// �߂�l: �I�������e�̂̓Y����
int Population::rouletteSelect()
{
	int rank;
	double prob, r;

	r = RAND_01;
	for(rank = 1; rank < POP_SIZE; rank++) {
		prob = trFit[rank - 1] / denom;
		if(r <= prob) {
			break;
		}
		r -= prob;
	}
	return rank - 1;
}

// �g�[�i�����g�I���Őe�̂�1�I������
// �߂�l: �I�������e�̂̓Y����
int Population::tournamentSelect()
{
	int i, ret, num, r;
	double bestFit;
	int tmp[N];

	for(i = 0; i < N; i++) {
		tmp[i] = 0;
	}
	ret = -1;
	bestFit = DBL_MAX;
	num = 0;
	while(1) {
		r = rand() % N;
		if(tmp[r] == 0) {
			tmp[r] = 1;
			if(ind[r]->fitness < bestFit) {
				ret = r;
				bestFit = ind[r]->fitness;
			}
			if(++num == TOURNAMENT_SIZE) {
				break;
			}
		}
	}
	return ret;
}

// ���ʂ�\������
void Population::printResult()
{
	int i;

	printf("�W��A�F");
	for(i = 0; i < N; i++) {
		if(ind[0]->chrom[i] == 1) {
			printf("��%d ", i + 1);
		}
	}
	printf("\n�W��B�F");
	for(i = 0; i < N; i++) {
		if(ind[0]->chrom[i] == 0) {
			printf("��%d ", i + 1);
		}
	}
	printf("\n���F%f\n", ind[0]->fitness);
}
