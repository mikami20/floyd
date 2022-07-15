#include "Population.h"

#define MAXNAME 20//
//#define SET_ON   //最終的なグループ分けの内訳をSet.txtに出力
#define ROOP_ON  //

int main()
{
#ifdef ROOP_ON
	for(int z = 1; z <= 20; z++) {
#endif // ROOP_ON
	int i;
	double best_fit = DBL_MAX;  //double型変数の最大値に変換，とりあえず最大値にしておけば第１世代が必ず最良解になる
	Population* pop; //popはポインタ, アドレスが入る, *が無ければpop自体がPopulation, popというオブジェクト(Population)を指し示す箱が出来ただけ
	clock_t start, end;  //clock測定関数
	float comptime;  //実行時間
	//char filename[MAXNAME];//付け足し
	FILE* fp1;

#ifdef SET_ON
	FILE* fp2;
#endif // SET_ON

	srand((unsigned int)time(NULL)); //unsigned：符号なし整数型 ，srand関数に現在時刻を渡すことで初期化している

	start = clock(); //CPU時間測定関数, プロセス開始からのクロック数を返す
	pop = new Population(); // newはオブジェクトを作れということ, newしただけでコンストラクタが動き，芋づる式で全て実行される, ここで初めてオブジェクトを作成
	for(i = 1; i <= GEN_MAX; i++) {  // 世代交代数, 現在の設定では1000
		pop->alternate();  //「popに対してalternateを実行しろ」という命令, alternateはPopulationに入っている, alternateは世代交代をする関数
		if(best_fit > pop->ind[0]->fitness) { //Population.cppでindというPOP SIZE個要素を持った配列を作成済み, ind[0]はエリート
			best_fit = pop->ind[0]->fitness;
			printf("第%d世代：最良適応度%f\n", i, best_fit); //世代交代して良いものが出来たら表示する，途中で表示すると動いている感がある
		}
	}
	end = clock();
	comptime = (float)(end-start) / CLOCKS_PER_SEC;  //「終了時刻 - 開始時刻 = 実行時間」, 秒に変換している
	printf("処理時間：%f秒\n", comptime);
	pop->printResult(); //関数printResultはPopulationで定義
	
	fp1 = fopen("Result.csv", "a");
	fprintf(fp1, "%f, ", comptime);
	fprintf(fp1, "%f\n", pop->ind[0]->fitness);
	fclose(fp1);

#ifdef SET_ON
	fp2 = fopen("Set.txt", "a");
	fprintf(fp2, "集合A：");
	for (i = 0; i < N; i++) {
		if (pop->ind[0]->chrom[i] == 1) {
			fprintf(fp2, "√%d ", i + 1);
		}
	}
	fprintf(fp2, "\n集合B：");
	for (i = 0; i < N; i++) {
		if (pop->ind[0]->chrom[i] == 0) {
			fprintf(fp2, "√%d ", i + 1);
		}
	}
	fprintf(fp2, "\n\n");
	fclose(fp2);
#endif

	delete pop;  //オブジェクトを削除，ここでデストラクタが実行される
#ifdef ROOP_ON
	}
#endif // ROOP_ON

	return 0;
}
