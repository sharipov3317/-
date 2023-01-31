using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace gpsch
{
    class Program
    {   //переменные
        static int a = 12;
        static int b = 21;
        static int c0 = 0;

        static int NOD(int a, int b)
        {
            while (a > 0 && b > 0)
            {
                if (a > b)
                    a %= b;
                else
                    b %= a;
            }
            return a + b;
        } //функция случайного выбора параметра для ГПСЧ
        static void GenerateRandomParameters()
        {
           DateTime start = new DateTime(2022, 12, 25);
           DateTime end = DateTime.Now;
           var secondss = double.Parse(((end - start).TotalSeconds).ToString());
           var seconds = Math.Abs((int)secondss);
            // вычисляем следующее случайное число
            a = seconds % 100;
            bool flag = false;
            while (flag == false)
            {
                if (a % 4 == 1) // а % 4 должно быть равно 1
                {
                    flag = true;
                }
                else
                {
                    a++;
                }
            }
            // вычисляем следующее случайное число
            b = (seconds / 60) % 10000;
            flag = false;
            while (flag == false)
            {
                int k = NOD(16777216, b);
                if (k == 1)
                {
                    flag = true;
                }
                else
                {
                    b++;
                }
            }

            c0 = (seconds / 3600) % 100000;
            Console.WriteLine("a = {0}", a);
            Console.WriteLine("b = {0}", b);
            Console.WriteLine("c0 = {0}", c0);
        }
        //Получаем следующий случайный номер
        static int GetNextRandomNumber(int a, int b, int c)
        {
            int ci = (a * c + b) % 16777216;
            return ci;
        }

        static string GetCorrectInput()
        {
            string str;

            while (true)
            {
                str = Console.ReadLine();//Читаем строку

                if (str.Length != 1)
                {
                    Console.WriteLine("Введите одно число 1 2 или 3");
                    continue;
                }

                if (str[0] != '1' && str[0] != '2' && str[0] != '3')
                {
                    Console.WriteLine("Введите одно число 1 2 или 3");
                    continue;
                }

                break;
            }

            return str;
        }
        static void Main(string[] args)
        {
            GenerateRandomParameters();//Генерируем случайные параметры
            int ci2 = c0;
            int ci3 = c0;
            while (true)
            {   //Вводым наш сообшение
                Console.WriteLine("Введите цифру 1 2 или 3");
                Console.WriteLine("(1) введите 1 чтобы сгенерировать новые параметры для гпсч");
                Console.WriteLine("(2) введите 2 чтобы получить ГПСЧ");
                Console.WriteLine("(3) введите 3 чтобы сгенерировать определенное количество чисел и записать в файл");
                
                var str = GetCorrectInput();
                if (str[0] == '1')
                    GenerateRandomParameters();
                if (str[0] == '2')
                {
                    Console.WriteLine(GetNextRandomNumber(a, b, ci2));
                    ci2 = GetNextRandomNumber(a, b, ci2);
                }
                if (str[0] == '3')
                {
                    while (true)
                    {
                        str = Console.ReadLine();//Читаем строку
                        bool f = false;
                        foreach (var w in str)
                        {
                            if (!Char.IsNumber(w))
                            {
                                f = true;
                                break;
                            }
                               
                        }
                        if (f)
                        {
                            Console.WriteLine("Введите одно число");//после 3
                            continue;
                        }

                        break;
                    }
                    var gistogram = new int[100];
                    var output = new StreamWriter("output.txt");//записываем число в файл
                    int countLast = 0;
                    for (int i = 0; i < int.Parse(str); i++)
                    {
                        output.WriteLine(GetNextRandomNumber(a, b, ci3));
                        ci3 = GetNextRandomNumber(a, b, ci3);
                        bool flag = true;
                        for (int j = 1; j < 99; j++)
                        {
                            if (ci3 < 167773 * j && ci3 > 167773 * (j - 1))
                            {
                                gistogram[j]++;
                                flag = false;
                            }
                        }
                        if (flag)
                            countLast++;
                    }
                    gistogram[99] = countLast;//построем гистаграммы
                    int max = 0;
                    for (int i = 0; i < 100; i++)
                    {
                        if (gistogram[i] > max)
                        {
                            max = gistogram[i];
                        }
                    }

                    for (int i = max + 1; i > 0; i--)
                    {
                        Console.Write("|");
                        for (int j = 0; j < 100; j++)
                        {
                            if (gistogram[j] == i)
                            {
                                gistogram[j]--;
                                Console.Write("*");
                            }
                            else
                            {
                                Console.Write(" ");
                            }
                        }
                        Console.WriteLine();
                    }
                    for (int i = 0; i < 101; i++)
                        Console.Write("_");
                    output.Close();
                }
            }
        }
    }
}
