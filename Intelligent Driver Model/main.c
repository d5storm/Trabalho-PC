#include <stdio.h>
#include <stdlib.h>




typedef struct veiculo{
    double position;
    double length;
    double velocity;
    double desiredVel;
    double timeSafety;
    double a;
    double b;
    struct veiculo * posterior;
}Veiculo;

typedef struct lista{
    int n;
    Veiculo * first;
    Veiculo * last;
}ListaVeiculos;

double roadLength, minimumDistance;


int main()
{
    double desiredVel, timeSafety,a,b,length;
    int i;
    /*printf("Escreva a distancia minima desejada entre cada carro: ");
    scanf(" %lf",&minimumDistance);
    printf("Escreva o tamanho da estrada: ");
    scanf(" %lf",&roadLength);*/
    minimumDistance = 5;
    roadLength = 200;
    Veiculo * car;
    ListaVeiculos * lista = (ListaVeiculos*)malloc(sizeof(ListaVeiculos));
    lista->n = 0;
    lista->first = NULL;
    lista->last = NULL;
    for(i = 0; i < 3; i++){
        /*printf("Escreva a velocidade desejada de cada carro %d: ",i);
        scanf(" %lf",&desiredVel);
        printf("Escreva o tempo de reaçao desejada de cada carro %d: ",i);
        scanf(" %lf",&timeSafety);
        printf("Escreva a acelaracao max desejada de cada carro %d: ",i);
        scanf(" %lf",&a);
        printf("Escreva a desacelaracao desejada de cada carro %d: ",i);
        scanf(" %lf",&b);
        printf("Escreva o tamanho desejado de cada carro %d: ",i);
        scanf(" %lf",&length);*/
        desiredVel = 10;
        timeSafety = 1.5f;
        length = 5;
        a = 1;
        b = 3;
        car = (Veiculo*)malloc(sizeof(Veiculo));
        car->a = a;
        car->b = b;
        car->desiredVel = desiredVel;
        car->timeSafety = timeSafety;
        car->length = length;
        car->velocity = 0;
        car->posterior = NULL;
        if(lista->last == NULL){
            car->position = 0;
            lista->first = car;
            lista->last = car;
            lista->n++;
        } else{
            car->position = lista->last->position + lista->last->length + minimumDistance;
            lista->last->posterior = car;
            lista->last = car;
            lista->n++;
        }
    }
    //printando a lista para teste
    lista->last->posterior = lista->first;
    for(car = lista->first; car->posterior != NULL; car = car->posterior){
        printf("position = %lf, length = %lf, velocity = %lf, desiredVel = %lf, timeSafety = %lf, a = %lf, b = %lf",car->position,car->length,car->velocity ,car->desiredVel ,car->timeSafety ,car->a ,car->b);
        printf("\n***************\n");
    }
    printf("position = %lf, length = %lf, velocity = %lf, desiredVel = %lf, timeSafety = %lf, a = %lf, b = %lf",car->position,car->length,car->velocity ,car->desiredVel ,car->timeSafety ,car->a ,car->b);
    printf("\n");


    return 0;
}
