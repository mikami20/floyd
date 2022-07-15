#include "Population.h"  //Population = 母集団，個体群

// コンストラクタ, newされると実行される
Population::Population()  //Populationクラスの関数であるPopulation()を定義している
{
	int i;

	ind = new Individual* [POP_SIZE]; //POP SIZE個要素を持った配列が出来あがる, POP SIZEはindividual.hに記載されている, POP SIZEは個体群のサイズ, 現在1000
	nextInd = new Individual* [POP_SIZE];  //次世代
	for(i = 0; i < POP_SIZE; i++) {
		ind[i] = new Individual();  //Individualのオブジェクトを指すようにする, ind[]という配列は個体を表す染色体を要素に持つ
		nextInd[i] = new Individual();  //nextIndは次の世代を作るとき用の場所
	}
	evaluate();
}

// デストラクタ
Population::~Population()  //最後のお掃除
{
	int i;

	for(i = 0; i < POP_SIZE; i++) {
		delete ind[i];
		delete nextInd[i];
	}
	delete [] ind;
	delete [] nextInd;
}

// すべての個体を評価して，適応度順に並び替える
void Population::evaluate()
{
	int i;

	for(i = 0; i < POP_SIZE; i++) {
		ind[i]->evaluate();  //これは，Individualのevaluate, ind[]の各配列に対して関数evaluateを実行しろと指示を出している
	}
	sort(0, POP_SIZE - 1);  //POP_SIZE個の配列に個体を入れているので添え字はPOP_SIZE - 1個
}

// ind[lb]～ind[ub]をクイックソートで並び替える
// lb: 並び替えの対象要素の添え字の下限
// ub: 並び替えの対象要素の添え字の上限
void Population::sort(int lb, int ub)
{
	int i, j, k;
	double pivot;  //pivot = 中心(点), 今回の場合はクイックソートの境界点として利用
	Individual *tmp;  //tmpはポインタ, アドレスが入る, tmpというオブジェクトを指し示す箱が出来ただけ

	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = ind[k]->fitness;  //個体kの適応度をpivotに代入
		i = lb;
		j = ub;
		do {
			while(ind[i]->fitness < pivot) {  //個体の適応度がpivotの適応度を上回るまでwhile文は続く
				i++;
			}
			while(ind[j]->fitness > pivot) {
				j--;
			}
			if(i <= j) {  //下から足しているiと，上から引いているjが上のwhile文を抜けてきたのにiの方が小さいときはpivotとの相対関係を入れ替える
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

// 世代交代をする
void Population::alternate()
{
	int i, j, p1, p2;
	Individual **tmp;  //tmpはポインタ, アドレスが入る, オブジェクトを指し示すtmpという箱が出来ただけ
	denom = 0.0;  // ルーレット選択の確率を求めるときの分母

	// ルーレット選択のための処理
	/*
	for(i = 0; i < POP_SIZE; i++) {
		trFit[i] = (ind[POP_SIZE - 1]->fitness - ind[i]->fitness) / (ind[POP_SIZE - 1]->fitness - ind[0]->fitness);
		denom += trFit[i];
	}
	*/

	// エリート保存戦略で子個体を作る
	for(i = 0; i < ELITE; i++) {  // エリート保存戦略で残す個体の数
		for(j = 0; j < N; j++) {
			nextInd[i]->chrom[j] = ind[i]->chrom[j];  //先に次世代に染色体をコピーしておく
		}
	}

	// 親を選択し交叉する
	for(; i < POP_SIZE; i++) {  //なぜiを初期化していないか分からない，不定値にならないのか？

		// 順位に基づくランキング選択
		p1 = rankingSelect1();
		p2 = rankingSelect1();
		/*
		// 確率に基づくランキング選択
		p1 = rankingSelect2();
		p2 = rankingSelect2();
		// ルーレット選択
		p1 = rouletteSelect();
		p2 = rouletteSelect();
		// トーナメント選択
		p1 = tournamentSelect();
		p2 = tournamentSelect();
		*/

		// 一点交叉
		nextInd[i]->crossover1(ind[p1], ind[p2]);  //crossover1はIndividualの関数
		/*
		// 二点交叉
		nextInd[i]->crossover2(ind[p1], ind[p2]);
		// 一様交叉
		nextInd[i]->crossoveru(ind[p1], ind[p2]);
		*/
	}

	// 突然変異を起こす，突然変異はエリートには起こさない
	for(i = 1; i < POP_SIZE; i++) {
		nextInd[i]->mutate();
	}

	// 次世代を現世代に変更する
	tmp = ind;
	ind = nextInd;
	nextInd = tmp;

	// 評価する
	evaluate();
}

// 順位に基づくランキング選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
int Population::rankingSelect1()
{
	int num, denom, r;  //このdenomの有効範囲はブロック有効範囲であり，ヘッダファイルで定義されているdenomとは別物

	denom = POP_SIZE * (POP_SIZE + 1) / 2;  //POP_SIZE = 個体群のサイズ, 個体の数，現在は1000
	r = ((rand() << 16) + (rand() << 1) + (rand() % 2)) % denom + 1;  //0 以上 denom 以下の乱数
	for(num = POP_SIZE; 0 < num; num--) {  
		if(r <= num) {
			break;
		}
		r -= num;
	}
	return POP_SIZE - num;  //POP_SIZE - num = 親個体の添え字
}

// 確率に基づくランキング選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
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

// ルーレット選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
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

// トーナメント選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
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

// 結果を表示する
void Population::printResult()
{
	int i;

	printf("集合A：");
	for(i = 0; i < N; i++) {
		if(ind[0]->chrom[i] == 1) {
			printf("√%d ", i + 1);
		}
	}
	printf("\n集合B：");
	for(i = 0; i < N; i++) {
		if(ind[0]->chrom[i] == 0) {
			printf("√%d ", i + 1);
		}
	}
	printf("\n差：%f\n", ind[0]->fitness);
}
