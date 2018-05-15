#include <stdio.h>
#include <conio.h>
#include <math.h>
int n = 15;
int clusters = 0;

struct point
{
	float x;
	float y;
	int cluster;
} *points;

// Вариант 3
//float x[15] = {88, 84, 92, 86, 96, 96, 10, 26, 35, 18, 14, 22, 71, 10, 13 };
//float y[15] = {35, 38, 39, 36, 26, 20, 17, 18, 13, 95, 93, 81, 77, 75, 78 };

// Вариант 25
//float x[15] = {82, 88, 84, 86, 81, 82, 78, 84, 84, 84, 78, 86, 85, 84, 78 };
//float y[15] = {85, 13, 18, 11, 27, 10, 14, 22, 13, 17, 14, 49, 97, 48, 39 };

// Вариант 31
float x[15] = {74, 79, 77, 82, 85, 82, 81, 78, 78, 79, 79, 71, 77, 81, 82 };
float y[15] = {78, 38, 10, 17, 36, 76, 13, 11, 22, 14, 75, 97, 84, 47, 39 };



float epsilon = 13.0f;				// Допуск
float d(int a, int b);				// Расстояние между точками

int main()
{
	int i, j, k, l, j_min, m;
	float min, minx, maxx, miny, maxy, tmp, sum;
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
	
	minx = miny = 0;
	maxx = maxy = 1;


	for(i = 0; i < n; i++)
	{
		points[i].x = (x[i] - minx) / (maxx - minx);
		points[i].y = (y[i] - miny) / (maxy - miny);
		points[i].cluster = -1;
	}

	// Объединение точек в кластеры
	l = 0;	// l - счетчик для кластеров
    for(i = 0; i < n; i++)
	{
		printf("\n\nCurrent point: %d\nTrying to add to existing cluster\n", i + 1); // Текущая точка
		
		min = 999;
		for(j = 0; j < l; j++)
		{
			printf("\nTrying to add to the cluster %d\n", j + 1); // Номер кластера
			printf("Point1\tPoint2\tDistance\n"); // Шапка

			sum = 0;
			m = 0;
			for(k = 0; k < n; k++)
			{
				if(points[k].cluster != j)
					continue;

				tmp = d(i, k);
				
				printf("%d\t%d\t%f\n", i + 1, k + 1, tmp); // Расстояния от точек кластера до заданной точки
				
				sum += tmp;
				m++;
				if(tmp > epsilon)
				{
					printf("Impossible to add the point because d(%d,%d) = %f > epsilon = %f\n",
						i + 1, k + 1, d(i, k), epsilon); // Нельзя увеличить кластер
					break;
				}
			}

			if(k == n)
				printf("Average distance is %f\n", sum / m);


			if(k == n && sum / m < min)
			{
				printf("All distances for this cluster are less than epsilon\n"); // Можно добавить точку

				min = sum / m;
				j_min = j;
			}
		}

		if(min < epsilon)
		{
			printf("\nThe minimal average distance is for cluster %d\n",
				j_min + 1); // Вывод epsilon
			printf("Adding the point %d to the cluster %d\n", i + 1, j_min + 1); // Сообщение о добавлении в кластер
			
			points[i].cluster = j_min;
		}
		else
		{
			printf("\nSome distances in clusters are more than epsilon = %f\n", epsilon); // Вывод epsilon
			printf("Can't add the point to clusters.\nCreate new cluster %d with the point %d\n",
				l + 1, i + 1); // Сообщение о новом кластере
			
			points[i].cluster = l;
			l++;
		}
	}

	// Вывод результатов
	printf("It is done. Points and clusters:\nPoint\tx\ty\tCluster\n");
	
	for(i = 0; i < n; i++)
		printf("%d\t%f\t%f\t%d\n",
			i + 1, points[i].x, points[i].y, points[i].cluster + 1);

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