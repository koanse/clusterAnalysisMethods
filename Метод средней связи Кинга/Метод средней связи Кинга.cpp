#include <stdio.h>
#include <conio.h>
#include <math.h>
int n = 15;
int clusters = 0;

//Вариант 3
float x[15] = {88, 84, 92, 86, 96, 96, 10, 26, 35, 18, 14, 22, 71, 10, 13 };
float y[15] = {35, 38, 39, 36, 26, 20, 17, 18, 13, 95, 93, 81, 77, 75, 78 };

// Вариант 25
//float x[15] = {82, 88, 84, 86, 81, 82, 78, 84, 84, 84, 78, 86, 85, 84, 78 };
//float y[15] = {85, 13, 18, 11, 27, 10, 14, 22, 13, 17, 14, 49, 97, 48, 39 };

// Вариант 31
//float x[15] = {74, 79, 77, 82, 85, 82, 81, 78, 78, 79, 79, 71, 77, 81, 82 };
//float y[15] = {78, 38, 10, 17, 36, 76, 13, 11, 22, 14, 75, 97, 84, 47, 39 };

float epsilon = .30;				// Допуск
float d(int a, int b);				// Расстояние между точками
float d_clusters(int a, int b);		// Расстояние между кластерами
void unit_clusters(int a, int b);	// Объединение двух кластеров (a < b)
void fill_cluster(int a);			// Заполнение кластера
bool is_free(int a);				// Проверка принадлежности точки кластеру
int points_in_cluster(int a);		// Количество точек в кластере

struct point
{
	float x;
	float y;
	int cluster;
} *points;

int main()
{
	int i, j, i_min, j_min;
	float min, tmp, minx, maxx, miny, maxy;
	bool exist_free_points = true;
	points = new struct point[n];

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
		
	//minx = miny = 0;
	//maxx = maxy = 1;
	for(i = 0; i < n; i++)
	{
		points[i].x = (x[i] - minx) / (maxx - minx);
		points[i].y = (y[i] - miny) / (maxy - miny);
		points[i].cluster = -1;
	}

	// Объединение точек в кластеры
	while(exist_free_points)
	{
        printf("\nTrying to create a new cluster\n"); // Сообщение о попытке создания нового кластера
		printf("Point1\tPoint2\tDistance\n"); // Шапка таблицы
		
		exist_free_points = false;
		min = 9999999999;
		for(i = 0; !exist_free_points && i < n; i++)
			if(is_free(i))
				exist_free_points = true;

		if(!exist_free_points)
			continue;
		
		for(i--; i < n - 1; i++)
		{
			if(!is_free(i))
				continue;

			for(j = i + 1; j < n; j++)
			{
				if(!is_free(j))
					continue;

				tmp = d(i, j);

				printf("%d\t%d\t%f\n", i + 1, j + 1, tmp); // Расстояния между "свободными" точками

				if(tmp < min)
				{
					min = tmp;
					i_min = i;
					j_min = j;
				}
			}
		}
		
		printf("Epsion is %f\n", epsilon); // Допуск
		printf("Minimal distance is %f\n", min); // Минимальное расстояние
		

		if(min < epsilon)
		{
			clusters++;
			points[i_min].cluster = points[j_min].cluster = clusters;
			
			printf("Creating the cluster %d with points %d and %d\n", clusters,
				i_min + 1, j_min + 1); // Сообщение о новом кластере

			fill_cluster(clusters);
		}
		else
		{
            exist_free_points = false;

			printf("Can't create a new cluster\n"); // Сообщение о невозможности создания нового кластера
		}
	}

	// Объединение кластеров
	min = epsilon - 1;

	while(min < epsilon)
	{
        printf("\nTrying to unit clusters\n"); // Сообщение о попытке объединения кластеров
		printf("Cluster1\tCluster2\tDistance\n"); // Шапка таблицы
		
		min = epsilon + 1;
		for(i = 1; i < clusters; i++)
			for(j = i + 1; j < clusters + 1; j++)
			{
				tmp = d_clusters(i, j);

				printf("%d\t\t%d\t\t%f\n", i, j, tmp); // Расстояния между кластерами

				if(tmp < min)
				{
					min = tmp;
					i_min = i;
					j_min = j;
				}
			}

		printf("Epsion is %f\n", epsilon); // Допуск

		if(min < epsilon)
		{
			unit_clusters(i_min, j_min);

			printf("Uniting clusters %d and %d\n", i_min, j_min); // Сообщение об объединении кластеров

		}
		else
			printf("Can't unit any more clusters\n"); // Сообщение о невозможности объединения кластеров
	}

	// Вывод результатов
	printf("It is done. Points and clusters:\n");
	
	for(i = 0; i < n; i++)
		printf("%d\t%f\t%f\t%d\n", i + 1, points[i].x, points[i].y, points[i].cluster);

	getch();
	


	return 0;
}

float d(int a, int b)
{
	float res = 0;
	res += (points[a].x - points[b].x) * (points[a].x - points[b].x);
	res += (points[a].y - points[b].y) * (points[a].y - points[b].y);
    return powf(res, 0.5);
}

float d_clusters(int a, int b)
{
	int i, n1 = 0, n2 = 0;
	float avx1 = 0, avx2 = 0, avy1 = 0, avy2 = 0, res = 0;
	for(i = 0; i < n; i++)
	{
		if(points[i].cluster == a)
		{
			n1++;
			avx1 += points[i].x;
			avy1 += points[i].y;
			continue;
		}

		if(points[i].cluster == b)
		{
			n2++;
			avx2 += points[i].x;
			avy2 += points[i].y;
			continue;
		}
	}

	avx1 /= n1; avy1 /= n1;
	avx2 /= n2; avy2 /= n2;

	res += (avx1 - avx2) * (avx1 - avx2) + (avy1 - avy2) * (avy1 - avy2);
    return powf(res, 0.5);
}

void unit_clusters(int a, int b)
{
	int i;
	for(i = 0; i < n; i++)
		if(points[i].cluster == b)
			points[i].cluster = a;

	for(i = 0; i < n; i++)
		if(points[i].cluster > b)
			points[i].cluster--;
	return;
}

void fill_cluster(int a)
{
	int i, j, i_min, j_min, p = points_in_cluster(a);
	float min = epsilon - 1, av;
	while(min < epsilon)
	{
		printf("Trying to add a point to cluster %d\n", a); // Сообщение о попытке увеличения кластера
		printf("Point\tDistance\n"); // Шапка таблицы

		
		min = epsilon + 1;
		
		for(i = 0; i < n; i++)
		{
			if(points[i].cluster == -1)
			{
				av = 0;
				for(j = 0; j < n; j++)
					if(points[j].cluster == a)
						av += d(i, j);

                av /= p;

				printf("%d\t%f\n", i + 1, av); // Средние расстояния до точки-кандидата

				if(av < min)
				{
					min = av;
					i_min = i;
					j_min = j;
				}
			}
		}

		printf("Epsion is %f\n", epsilon); // Допуск

		if(min < epsilon)
		{
			points[i_min].cluster = a;
			p++;

			printf("Adding the point %d to the cluster %d\n", i_min + 1, a); // Сообщение о добавлении точки
		}
		else
			printf("Can't add points to the cluster %d\n", a); // Сообщение о невозможности увеличить кластер
	}
	
	return;
}

bool is_free(int a)
{
	if(points[a].cluster == -1)
		return true;
	return false;
}


int points_in_cluster(int a)
{
	int i, j = 0;
	for(i = 0; i < n; i++)
		if(points[i].cluster == a) j++;
	return j;
}
