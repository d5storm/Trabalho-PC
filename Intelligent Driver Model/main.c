#include <stdio.h>
#include <stdlib.h>
#include <math.h>




typedef struct veiculo{
    int id;
    double position;
    double length;
    double velocity;
    double desiredVel;
    double timeSafety;
    double maxA;
    double desiredA;
    struct veiculo * posterior;
}Veiculo;

typedef struct lista{
    int n;
    Veiculo * first;
    Veiculo * last;
}ListaVeiculos;
//roadLength = 100
//if(carro na frente tiver pos < minha)
//tamanho da pista + pos do carro na frente





double roadLength, minimumSpacing,accelExp;
Veiculo * ghost;
/*
fEspaço -> fVelocidade -> fAceleração
f' = Velocidade;
f'' = a(1-.......);

f' = W;
f'' = W';

W = velocidade;
W' = a(1-.......;

Yn+1 = Yn + h/2(R1)
R1 = Yn + h(F); -> euler

F = [velocidade ]
    [a(1-.......)]*/
double sEstrela(double maxA, double desiredA, double timeSafety, double vA, double deltaVA,double T){

    return minimumSpacing + vA*T + ((vA*deltaVA) / (2* sqrt(maxA * desiredA)));

}

double aceleracao(Veiculo * atual){
    double deltaVA = atual->velocity - atual->posterior->velocity;
    double S = sEstrela(atual->maxA,atual->desiredA,atual->timeSafety,atual->velocity,deltaVA,atual->timeSafety);
    //printf("ENTROU ACELERACAO");
    double potencia = atual->velocity / atual->desiredVel;
    double posPosterior = atual->posterior->position;
    double Sa = posPosterior - atual->position - atual->posterior->length;
    double potencia2 = S/Sa;
    double accel = atual->desiredA*(1 - pow(potencia,accelExp) - pow(potencia2,2));
    //printf("desiredA: %lf potencia: %lf potencia2: %lf accelExp: %lf\n",atual->desiredA,potencia,potencia2,accelExp);
   // printf("DeltaVA: %lf S: %lf Accel = %lf\n",deltaVA,S,accel);
    return accel;
}

double EulerVelocidade(Veiculo * atual, double F[], double h){
    return atual->position + h*(F[0]);
}
double EulerAceleracao(Veiculo * atual, double F[], double h){
    return atual->velocity + h*(F[1]);
}
void printCar(Veiculo * atual){
    int pos = atual->position;
    printf("ID: %d Pos: %lf Vel: %lf",atual->id,atual->position,atual->velocity);
    printf("\n");
}

void rungeKutta2(ListaVeiculos * lista){
    //programar euler modificado;
    int i;
    double h;
    int fim = 1;
    for(h = 0;h < 100; h = h + 1){
        Veiculo * atual = lista->first;
        for(i = 0; i < lista->n; i++){
            if(atual->id == 3)
                printf("!!!!!!!!!!!!!!!!!!!!!!!");
            double accel = aceleracao(atual);

            double F[] = {
                atual->velocity,
                accel
            };

            double rUM[] = {
                EulerVelocidade(atual,F,h),
                EulerAceleracao(atual,F,h)
            };

            double velEuler = rUM[0];
            double accelEuler = rUM[1];
            atual->position = atual->position + (h/2)*velEuler;
            atual->velocity = atual->velocity + (h/2)*accelEuler;
            if(atual->id == 0){
                ghost->position = ghost->position + (h/2)*velEuler;
                ghost->velocity = ghost->velocity + (h/2)*accelEuler;
            }
            printCar(atual);
            atual = atual->posterior;
            //printf("***************************\n");
        }
        printf("ID: G Pos: %lf Vel: %lf\n",ghost->position,ghost->velocity);
        printf("-------------------------------\n");
        getchar();
    }

}

void BDF2(ListaVeiculos * lista){

}




int main()
{
    accelExp = 4;
    double desiredVel, timeSafety,a,b,length;
    int i;
    /*printf("Escreva a distancia minima desejada entre cada carro: ");
    scanf(" %lf",&minimumDistance);
    printf("Escreva o tamanho da estrada: ");
    scanf(" %lf",&roadLength);*/
    minimumSpacing = 2;
    roadLength = 201;
    Veiculo * car;
    ListaVeiculos * lista = (ListaVeiculos*)malloc(sizeof(ListaVeiculos));
    lista->n = 0;
    lista->first = NULL;
    lista->last = NULL;

    for(i = 0; i < 4; i++){
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
        desiredVel = 30;
        timeSafety = 1.5f;
        length = 5;
        a = 1;
        b = 3;
        car = (Veiculo*)malloc(sizeof(Veiculo));
        car->id = i;
        car->maxA = a;
        car->desiredA = b;
        car->desiredVel = desiredVel;
        car->timeSafety = timeSafety;
        car->length = length;
        car->velocity = 0;
        car->posterior = NULL;
        if(lista->last == NULL){
            ghost = (Veiculo*)malloc(sizeof(Veiculo));
            ghost->id = -1;
            ghost->maxA = a;
            ghost->desiredA = b;
            ghost->desiredVel = desiredVel;
            ghost->timeSafety = timeSafety;
            ghost->length = length;
            ghost->velocity = 0;
            ghost->posterior = NULL;
            ghost->position = 1 + roadLength;
            car->position = 1;
            lista->first = car;
            lista->last = car;
            lista->n++;
        } else{
            car->position = lista->last->position + 30;
            lista->last->posterior = car;
            lista->last = car;
            lista->n++;
        }
        printf("car position = %lf |", car->position);
    }
    //printando a lista para teste
    lista->last->posterior = ghost;

    printf("\nEntrou no RungeKutta!\n");
    rungeKutta2(lista);


    return 0;
}
