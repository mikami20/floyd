#include "Population.h"

#define MAXNAME 20//
//#define SET_ON   //�ŏI�I�ȃO���[�v�����̓����Set.txt�ɏo��
#define ROOP_ON  //

int main()
{
#ifdef ROOP_ON
	for(int z = 1; z <= 20; z++) {
#endif // ROOP_ON
	int i;
	double best_fit = DBL_MAX;  //double�^�ϐ��̍ő�l�ɕϊ��C�Ƃ肠�����ő�l�ɂ��Ă����Α�P���オ�K���ŗǉ��ɂȂ�
	Population* pop; //pop�̓|�C���^, �A�h���X������, *���������pop���̂�Population, pop�Ƃ����I�u�W�F�N�g(Population)���w�����������o��������
	clock_t start, end;  //clock����֐�
	float comptime;  //���s����
	//char filename[MAXNAME];//�t������
	FILE* fp1;

#ifdef SET_ON
	FILE* fp2;
#endif // SET_ON

	srand((unsigned int)time(NULL)); //unsigned�F�����Ȃ������^ �Csrand�֐��Ɍ��ݎ�����n�����Ƃŏ��������Ă���

	start = clock(); //CPU���ԑ���֐�, �v���Z�X�J�n����̃N���b�N����Ԃ�
	pop = new Population(); // new�̓I�u�W�F�N�g�����Ƃ�������, new���������ŃR���X�g���N�^�������C���Â鎮�őS�Ď��s�����, �����ŏ��߂ăI�u�W�F�N�g���쐬
	for(i = 1; i <= GEN_MAX; i++) {  // �����㐔, ���݂̐ݒ�ł�1000
		pop->alternate();  //�upop�ɑ΂���alternate�����s����v�Ƃ�������, alternate��Population�ɓ����Ă���, alternate�͐����������֐�
		if(best_fit > pop->ind[0]->fitness) { //Population.cpp��ind�Ƃ���POP SIZE�v�f���������z����쐬�ς�, ind[0]�̓G���[�g
			best_fit = pop->ind[0]->fitness;
			printf("��%d����F�ŗǓK���x%f\n", i, best_fit); //�����サ�ėǂ����̂��o������\������C�r���ŕ\������Ɠ����Ă��銴������
		}
	}
	end = clock();
	comptime = (float)(end-start) / CLOCKS_PER_SEC;  //�u�I������ - �J�n���� = ���s���ԁv, �b�ɕϊ����Ă���
	printf("�������ԁF%f�b\n", comptime);
	pop->printResult(); //�֐�printResult��Population�Œ�`
	
	fp1 = fopen("Result.csv", "a");
	fprintf(fp1, "%f, ", comptime);
	fprintf(fp1, "%f\n", pop->ind[0]->fitness);
	fclose(fp1);

#ifdef SET_ON
	fp2 = fopen("Set.txt", "a");
	fprintf(fp2, "�W��A�F");
	for (i = 0; i < N; i++) {
		if (pop->ind[0]->chrom[i] == 1) {
			fprintf(fp2, "��%d ", i + 1);
		}
	}
	fprintf(fp2, "\n�W��B�F");
	for (i = 0; i < N; i++) {
		if (pop->ind[0]->chrom[i] == 0) {
			fprintf(fp2, "��%d ", i + 1);
		}
	}
	fprintf(fp2, "\n\n");
	fclose(fp2);
#endif

	delete pop;  //�I�u�W�F�N�g���폜�C�����Ńf�X�g���N�^�����s�����
#ifdef ROOP_ON
	}
#endif // ROOP_ON

	return 0;
}
