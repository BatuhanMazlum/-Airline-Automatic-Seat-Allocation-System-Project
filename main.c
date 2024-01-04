#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define INF -9999999999.0

int COL_CAP; //column capacity, to be able to pass arrays to functions.

//setting all elements to "0"
void reset_int_array(int seats[][COL_CAP],int rows, int COL_CAP)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < COL_CAP; j++)
        {
            seats[i][j] = 0;
        }
    }
}

//setting all elements to "0"
void reset_double_array(long double score_array[][COL_CAP],int rows, int COL_CAP)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < COL_CAP; j++)
        {
            score_array[i][j] = INF;
        }
    }
}

//to define seat letters
char num_to_char(int seats[][COL_CAP], int column, int COL_CAP)
{
    int k = 0;
    char letter;

    for(int i = 0; i < column; i++)
    {
        if(seats[0][i] != 33)
        {
            k++;
        }
    }

    letter = k + 65;

    return letter;
}

//to print plane layout
void print_layout(int seats[][COL_CAP], int rows, int COL_CAP, int letter_row[])
{
    char num_to_char;
    int r = 0;

    printf("\n");
    printf("   ");

    for(int i = 0; i < COL_CAP; i++)
    {
        if(letter_row[i] != 33)
        {
            num_to_char = r + 65;
            printf("%c ", num_to_char);
        }
        else
        {
            printf("|  | ");
        }
        if(letter_row[i] != 33)
        {
            r++;
        }
    }
    printf(" ");

    printf("\n");

    for(int i = 0; i < rows; i++)
    {
        if(i < 9)
        {
            printf("%d  ", i + 1);
        }
        else
        {
            printf("%d ", i + 1);
        }
        for(int j = 0; j < COL_CAP; j++)
        {
            if(seats[i][j] == 0)
            {
                printf("- ");
            }
            else if(seats[i][j] == 1)
            {
                printf("+ ");
            }
            else if(seats[i][j] == 2)
            {
                printf("X ");
            }
            else
            {
                printf("|  | ");
            }
        }
        printf("\n");
    }
}

//to determine which single seat should be reserved
void highest_score(long double score_array[][COL_CAP], int rows, int COL_CAP, int *x, int *y, int seats[][COL_CAP])
{
    long double temp = -999999999.0;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < COL_CAP; j++)
        {
            if(score_array[i][j] > temp)
            {
                if(seats[i][j] != 33)
                {
                    if(seats[i][j] == 0)
                    {
                        temp = score_array[i][j];
                        *x = i;
                        *y = j;
                    }
                }
            }
        }
    }
}

//to determine which multiple seats should be reserved
void multi_highest_score(long double score_array[][COL_CAP], int rows, int COL_CAP, int *x, int *y, int seats[][COL_CAP], int people)
{
    long double temp = -999999999.0;
    int temp_aisle = 0;
    int aisle = 10;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < COL_CAP; j++)
        {
            if(score_array[i][j] >= temp)
            {
                if(seats[i][j] != 33)
                {
                    if(seats[i][j] == 0)
                    {
                        int p = j;
                        for(int k = 0; k < people; k++)
                        {
                            if(seats[i][p] == 33)
                            {
                                temp_aisle++;
                                k--;
                            }
                            p++;
                        }
                        if(score_array[i][j] == temp)
                        {
                            if(temp_aisle < aisle)
                            {
                                aisle = temp_aisle;
                                temp = score_array[i][j];
                                *x = i;
                                *y = j;
                            }

                        }
                        else
                        {
                            aisle = temp_aisle;
                            temp = score_array[i][j];
                            *x = i;
                            *y = j;
                        }
                        temp_aisle = 0;
                    }
                }
            }
        }
    }
}

//to compute adjusted distance
long double compute_adj_dist(int i, int j, int m, int n)
{
    long double dist = 0.0;
    long double adj_dist = 0.0;

    dist = ((long double) abs(i - m)) + (((long double) abs(j - n)) / 2.0);

    if(dist == 0)
    {
        adj_dist = 0.0;
    }
    else
    {
        adj_dist = 100.0 - ((long double) pow(100.0, 1/dist));
    }

    return adj_dist;
}

//to compute single seat score
long double compute_seat_score(int seats[][COL_CAP],int x, int y, int rows, int COL_CAP)
{
    long double total = 0.0;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < COL_CAP; j++)
        {
            if(seats[i][j] == 2)
            {
                total += compute_adj_dist(i, j, x, y);
            }
        }
    }

    return total;
}

//to compute multiple seat score
long double compute_multiple_seat_score(int seats[][COL_CAP],int x, int y, int rows, int COL_CAP, int people)
{
    long double total = 0.0;

    for(int k = 0; k < people; k++)
    {
        if(k != 0)
        {
            y++;
            if(seats[x][y] == 33)
            {
                y++;
            }
        }
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < COL_CAP; j++)
            {
                if(seats[i][j] == 2)
                {
                    total += compute_adj_dist(i, j, x, y);
                }
            }
        }
    }

    return total;
}

//to check if seats are available to reserve
int check_avb(int seats[][COL_CAP], int x, int y, int people, int seats_per_row, int ct)
{
    int avail = 0;

    if(seats_per_row - y + ct < people)
    {
        avail = 1;
    }
    else
    {
        for(int i = 0; i < people; i++)
        {
            if(seats[x][y] == 33)
            {
                i--;
            }
            if(seats[x][y] == 2)
            {
                avail = 1;
                break;
            }
            y++;
        }
    }

    return avail;
}

int main()
{
    int rows = 0;               //row number of the plane
    int seats_per_row = 0;      //seat number per row
    int aisles = 0;             //aisle number
    int temp_aisle = 0;
    int *cap_ptr = &COL_CAP;    //pointer to change global variable
    int user_input = 0;
    int highest_i = 0;          //row order of next reserved seat
    int highest_j = 0;          //column order of next reserved seat
    int count = 0;
    int *count_ptr = &count;

    do
    {
        printf("Number of rows: ");
        scanf("%d", &rows);
    }
    while(rows < 1 || rows > 99);

    do
    {
        printf("Number of seats per row: ");
        scanf("%d", &seats_per_row);
    }
    while(seats_per_row < 1 || seats_per_row > 20);

    do
    {
        printf("Number of aisles: ");
        scanf("%d", &aisles);
    }
    while(aisles < 1 || aisles > 5);

    *cap_ptr = seats_per_row + aisles;

    int seats[rows][COL_CAP];
    int letter_row[COL_CAP];
    long double score_array[rows][COL_CAP];

    reset_int_array(seats, rows, COL_CAP);
    reset_double_array(score_array, rows, COL_CAP);

    for(int i = 0; i < COL_CAP; i++)
    {
        letter_row[i] = i;
    }

    for(int i = 1; i <= aisles; i++)
    {
        int u = seats_per_row;
        printf("Add aisle %d after seat: ", i);
        scanf("%d", &temp_aisle);
        if(i != 1)
        {
            temp_aisle = temp_aisle + i - 1;
            u++;
        }
        for(int j = 0; j < rows; j++)
        {
            for(int k = u - 1; k >= temp_aisle; k--)
            {
                seats[j][k] = seats[j][k - 1];
            }
            seats[j][temp_aisle] = 33;
        }
        for(int m = u - 1; m >= temp_aisle; m--)
        {
            letter_row[m] = letter_row[m - 1];
        }
        letter_row[temp_aisle] = 33;
    }

    print_layout(seats, rows, COL_CAP, letter_row);

    while(true)
    {
        printf("The number of people in the reservation\n");
        printf(" (0: print current reservations, -1: exit): ");
        scanf("%d", &user_input);

        if(user_input == -1) //exit case
        {
            break;
        }
        else if(user_input == 0) //printing layout case
        {
            print_layout(seats, rows, COL_CAP, letter_row);
        }
        else if(user_input == 1) //single reservation case
        {
            for(int i = 0; i < rows; i++)
            {
                for(int j = 0; j < COL_CAP; j++)
                {
                    if(seats[i][j] == 0)
                    {
                        int temp2 = 0;
                        score_array[i][j] = compute_seat_score(seats, i, j, rows, COL_CAP);
                        temp2++;
                        *count_ptr = temp2;
                    }
                }
            }
            if(count != 0)
            {
                highest_score(score_array, rows, COL_CAP, &highest_i, &highest_j, seats);
                seats[highest_i][highest_j] = 1;
                printf("Reserved seats: %d%c\n", highest_i + 1, num_to_char(seats, highest_j, COL_CAP));
                print_layout(seats, rows, COL_CAP, letter_row);
                seats[highest_i][highest_j] = 2;
            }
            else
            {
                printf("No available seats for the requested reservation!\n");
            }
            reset_double_array(score_array, rows, COL_CAP);
            highest_i = 0;
            highest_j = 0;
            *count_ptr = 0;
        }
        else // multiple reservation case
        {
            for(int i = 0; i < rows; i++)
            {
                if(count == user_input)
                {
                    break;
                }
                *count_ptr = 0;
                for(int j = 0; j < COL_CAP; j++)
                {
                    *count_ptr = *count_ptr + 1;
                    if(seats[i][j] == 2)
                    {
                        *count_ptr = 0;
                    }
                    if(count == user_input)
                    {
                        break;
                    }
                }
            }

            if(count != user_input)
            {
                printf("No available seats for the requested reservation!\n");
            }
            else
            {
                int availability = 5;
                int cnt_avb = 0;
                int ais_ct = 0;
                for(int i = 0; i < rows; i++)
                {
                    ais_ct = 0;
                    for(int j = 0; j < COL_CAP; j++)
                    {
                        if(seats[i][j] == 33)
                        {
                            ais_ct++;
                        }
                        if(seats[i][j] != 33)
                        {
                            availability = check_avb(seats, i, j, user_input, seats_per_row, ais_ct);
                            if(availability == 0)
                            {
                                score_array[i][j] = compute_multiple_seat_score(seats, i, j, rows, COL_CAP, user_input);
                                cnt_avb++;
                            }
                        }
                    }
                }
                ais_ct = 0;
                if(cnt_avb == 0)
                {
                    printf("No available seats for the requested reservation!\n");
                }
                else
                {
                    multi_highest_score(score_array, rows, COL_CAP, &highest_i, &highest_j, seats, user_input);
                    int q = highest_j;
                    for(int z = 0; z < user_input; z++)
                    {
                        if(seats[highest_i][q] == 33)
                        {
                            z--;
                        }
                        if(seats[highest_i][q] == 0)
                        {
                            seats[highest_i][q] = 1;
                        }
                        q++;
                    }
                    printf("Reserved seats:");
                    q = highest_j;
                    for(int z = 0; z < user_input; z++)
                    {
                        if(seats[highest_i][q] == 33)
                        {
                            z--;
                        }
                        if(seats[highest_i][q] == 1)
                        {
                            printf(" %d%c", highest_i + 1, num_to_char(seats, q, COL_CAP));
                        }
                        q++;
                    }
                    printf("\n");
                    print_layout(seats, rows, COL_CAP, letter_row);
                    q = highest_j;
                    for(int z = 0; z < user_input; z++)
                    {
                        if(seats[highest_i][q] == 33)
                        {
                            z--;
                        }
                        if(seats[highest_i][q] == 1)
                        {
                            seats[highest_i][q] = 2;
                        }
                        q++;
                    }
                }
            }
            *count_ptr = 0;
            reset_double_array(score_array, rows, COL_CAP);
            highest_i = 0;
            highest_j = 0;
        }
    }

    return EXIT_SUCCESS;
}
