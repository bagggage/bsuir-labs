#include <stdio.h>

void clearInputBuffer()
{
    rewind(stdin);
}

void throwErrorAndClearInputBuffer()
{
    printf("\nIncorrect input, please try enter data again:\n");
    clearInputBuffer();
}

int main()
{
    //Task 1 (number 3) 
    {
        float costOfCapitalToday = 20.f;
        
        for (int i = 0; i < (2022 - 1626); i++)
            costOfCapitalToday += costOfCapitalToday * 0.04f;

        printf("If in 1626 $20 were deposited in a bank at 4%% per annum, today the cost of capital would be - $%.0f\n", costOfCapitalToday);
    }
    
    //Task 2 (number 3)
    {
        int i = 0;
        int countOfKopecksInRubel = 100;

        for (int a = 0; a <= 2; a++)
            for (int b = 0; b <= 5; b++)
                for (int c = 0; c <= 20; c++)
                    for (int d = 0; d <= 50; d++)
                        if (countOfKopecksInRubel == (a * 50 + b * 20 + c * 5 + d * 2))
                            i++;

        printf("\nThe number of ways to collect one ruble from kopecks with a face value of 50 kop., 20kop., 5kop., 2kop. - %i\n", i);
    }

    //Task 3 (number 3)
    {
        float a = 0, b = 0;

        printf("\nEnter length of 'A' and 'B' segments (A > B): ");

        while (scanf_s("%f %f", &a, &b) < 2 || (a <= 0 || a <= b))
            throwErrorAndClearInputBuffer();

        float tempSumOfBinA = 0;
        int i = 0;

        while (tempSumOfBinA < a)
        {
            tempSumOfBinA += b;

            if (tempSumOfBinA <= a)
                i++;
        }

        printf("Segment 'B' fits in segment 'A' %i times\n", i);
    }

    return 0;
}