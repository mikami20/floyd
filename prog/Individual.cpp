#include "Individual.h"  //Individual = 個体，個人

// コンストラクタ
Individual::Individual()
{
	int i;

	for(i = 0; i < N; i++) {
		chrom[i] = rand() % 2;  //個体の各染色体に0か1の乱数を入れる，1は集合Aに属する・0は集合Bに属する
	}
	fitness = 0.0;  //適応度を0で初期化，適応度は集合の差で表す
}

// デストラクタ
Individual::~Individual()
{
}

// 適応度を算出する
void Individual::evaluate()
{
	int i;

	fitness = 0.0;
	for(i = 0; i < N; i++) {  // 集合の要素となる最大数の平方値, 今回の場合64
		fitness += (chrom[i] * 2 - 1) * sqrt((double)i + 1);  //適応度関数！！
	}
	fitness = fabs(fitness);  //絶対値をとる
}

// p1とp2から一点交叉で作った子にする
// p1: 親個体1
// p2: 親個体2
void Individual::crossover1(Individual *p1, Individual *p2)
{
	int point, i;

	point = rand() % (N - 1);  //0からN-1以下の整数の乱数
	for(i = 0; i <= point; i++) {
		chrom[i] = p1->chrom[i];  //0からpointまでp1の遺伝子を渡す
	}
	for(; i < N; i++) {
		chrom[i] = p2->chrom[i];  //pointから63までp2の遺伝子を渡す
	}
}

// p1とp2から二点交叉で作った子にする
// p1: 親個体1
// p2: 親個体2
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

// p1とp2から一様交叉で作った子にする
// p1: 親個体1
// p2: 親個体2
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

// 突然変異を起こす
void Individual::mutate()
{
	int i;

	for(i = 0; i < N; i++) {  //全ての染色体に対して以下の操作を実施する
		if(RAND_01 < MUTATE_PROB) {  //0以上1以下の実数乱数が0.01以下になれば染色体を全て反転
			chrom[i] = 1 - chrom[i];
		}
	}
}
