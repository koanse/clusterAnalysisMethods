#include <stdio.h>
#include <conio.h>
#include <math.h>
#define a_big_number 9999999
#define n 15
int number_of_clusters = n;

struct point
{
	float x;
	float y;
} *points;

struct cluster
{
	struct cluster *left, *right;
	float d;
    int point;
} *clusters;

// Вариант 3
//float x[15] = {88, 84, 92, 86, 96, 96, 10, 26, 35, 18, 14, 22, 71, 10, 13 };
//float y[15] = {35, 38, 39, 36, 26, 20, 17, 18, 13, 95, 93, 81, 77, 75, 78 };

// Вариант 25
//float x[15] = {82, 88, 84, 86, 81, 82, 78, 84, 84, 84, 78, 86, 85, 84, 78 };
//float y[15] = {85, 13, 18, 11, 27, 10, 14, 22, 13, 17, 14, 49, 97, 48, 39 };

// Вариант 31
float x[15] = {74, 79, 77, 82, 85, 82, 81, 78, 78, 79, 79, 71, 77, 81, 82 };
float y[15] = {78, 38, 10, 17, 36, 76, 13, 11, 22, 14, 75, 97, 84, 47, 39 };

// Тестовый пример
//float x[15] = {41.5, 42.3, 42.0, 38.2, 39.3, 41.5, 45.5, 39.5, 42.0, 49.8, 39.2, 41.9, 45.6, 38.1, 44.2 };
//float y[15] = {26.5, 24.5, 24.5, 25.5, 26.0, 29.5, 30.0, 30.5, 30.5, 30.5, 31.0, 27.0, 27.5, 30.5, 30.5 };


float M1[n][n], M2[n][n];			// Матрицы расстояний
float d2(int a, int b);				// Квадрат расстояния между точками
float d2c(int h, int i, int j);		// Расстояние между кластером h и объединением кластеров i и j
void unit_clusters(int a, int b);	// Объединение кластеров
void copy_clusters(struct cluster *where, struct cluster *what);	// Копирование кластеров
int coordinate_in_M2_to_coordinate_in_M1(int i, int a, int b);		// Перевод координат
void print(struct cluster c);		// Вывод на экран кластера
void print_table();					// Вывод на экран таблицы

int main()
{
	int i, j, i_min, j_min, i1, j1;
	float min, minx, maxx, miny, maxy;
	points = new struct point[n];
	clusters = new struct cluster[n];

	// Нормировка
	minx = maxx = x[0];
	miny = maxy = y[0];

	for(i = 1; i < n; i++)
	{
		if(x[i] < minx) minx = x[i];
		if(x[i] > maxx) maxx = x[i];
		if(y[i] < miny) miny = y[i];
		if(y[i] > maxy) maxy = y[i];
	}
	
	minx = miny = 0;
	maxx = maxy = 1;


	for(i = 0; i < n; i++)
	{
		points[i].x = (x[i] - minx) / (maxx - minx);
		points[i].y = (y[i] - miny) / (maxy - miny);
		clusters[i].left = clusters[i].right = NULL;
		clusters[i].point = i;
	}

	
	// Заполнение первой матрицы расстояний
	
	for(i = 0; i < n; i++)
		for(j = 0; j < i + 1; j++)
			M1[i][j] = 0;
	
	min = a_big_number;
	for(i = 0; i < n - 1; i++)
		for(j = i + 1; j < n; j++)
		{
			M1[i][j] = d2(i, j);
			if(M1[i][j] < min)
			{
				min = M1[i][j];
				i_min = i;
				j_min = j;
			}
		}
	
	print_table();
	unit_clusters(i_min, j_min);

	// Объединение кластеров
	while(number_of_clusters > 1)
	{
		for(i = 0; i < number_of_clusters - 1; i++)
			for(j = i + 1; j < number_of_clusters; j++)
			{
				if(i == 0) M2[i][j] = d2c(j, i_min, j_min);
				else
				{
					i1 = coordinate_in_M2_to_coordinate_in_M1(i, i_min, j_min);
					j1 = coordinate_in_M2_to_coordinate_in_M1(j, i_min, j_min);
					M2[i][j] = M1[i1][j1];
				}
			}

		min = a_big_number;
		for(i = 0; i < number_of_clusters - 1; i++)
			for(j = i + 1; j < number_of_clusters; j++)
			{
				M1[i][j] = M2[i][j];
				if(M2[i][j] < min)
				{
					min = M2[i][j];
					i_min = i;
					j_min = j;
				}
			}
		print_table();
		
		unit_clusters(i_min, j_min);


	}


	// Вывод результатов
	printf("It is done. Points and clusters:\nPoint\tx\ty\tCluster\n");
	print(clusters[0]);
	printf("\n");
    
	for(i = 0; i < n; i++)
		printf("%d\t%f\t%f\n",
			i + 1, points[i].x, points[i].y);

	getch();

	return 0;
}

float d2(int a, int b)
{
	float res = 0;
	res += (points[clusters[a].point].x - points[clusters[b].point].x) *
		(points[clusters[a].point].x - points[clusters[b].point].x);
	res += (points[clusters[a].point].y - points[clusters[b].point].y) *
		(points[clusters[a].point].y - points[clusters[b].point].y);
	return res;
}
float d2c(int h, int i, int j)
{
	float res = 0, tmp1, tmp2;
    h = coordinate_in_M2_to_coordinate_in_M1(h, i, j);
	
	if(h < i) tmp1 = M1[h][i];
	else tmp1 = M1[i][h];
	
	if(h < j) tmp2 = M1[h][j];
	else tmp2 = M1[j][h];

	res += 0.5f * tmp1 + 0.5f * tmp2 + 0.5f * fabs(tmp1 - tmp2);
	return res;
}


void unit_clusters(int a, int b)
{
	int i;
	struct cluster *tmp1, tmp2;
	
	printf("\nUnit theese clusters: ");
	print(clusters[a]);
	printf(" and ");
	print(clusters[b]); // Сообщение об объединении кластеров
	printf("\n");

	tmp1 = new struct cluster;
	copy_clusters(tmp1, &(clusters[a]));
	tmp2.left = tmp1;
	
	tmp1 = new struct cluster;
	copy_clusters(tmp1, &(clusters[b]));
	tmp2.right = tmp1;

	tmp2.d = M1[a][b];
	tmp2.point = -1;

	for(i = 0; i < number_of_clusters - 2; i++)
	{
		if(i >= a && i < b - 1)
			copy_clusters(&(clusters[i]), &(clusters[i + 1]));

		if(i >= b - 1)
			copy_clusters(&(clusters[i]), &(clusters[i + 2]));
	}

	for(i = number_of_clusters - 2; i > 0; i--)
		copy_clusters(&(clusters[i]), &(clusters[i - 1]));

	copy_clusters(&(clusters[0]), &tmp2);
	number_of_clusters--;
	return;
}

void copy_clusters(struct cluster *where, struct cluster *what)
{
	where->d = what->d;
	where->left = what->left;
	where->right = what->right;
	where->point = what->point;
	return;
}


int coordinate_in_M2_to_coordinate_in_M1(int i, int a, int b)
{
	if(i <= a) return i - 1;
	if(i > a && i < b) return i;
	if(i >= b) return i + 1;
}
void print(struct cluster c)
{
	if(c.point != -1)
		printf("%d", c.point + 1);
	else
	{
		printf("(");
		print(*c.left);
		printf("; ");
		print(*c.right);
	}
	return;
}

void print_table()
{
	int i, j;
	
	printf("\t");
	for(i = 0; i < number_of_clusters; i++)
	{
        print(clusters[i]);
		printf("\t");
	}
	printf("\n");
	
	for(i = 0; i < number_of_clusters - 1; i++)
	{
		print(clusters[i]);
		printf("\t");
		for(j = 0; j < i + 1; j++)
			printf("\t");
		for(j = i + 1; j < number_of_clusters; j++)
			printf("%.2f\t", M1[i][j]);
		printf("\n");
	}
	return;
}
