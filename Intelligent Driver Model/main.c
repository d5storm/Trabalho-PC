#include <stdio.h>
#include <stdlib.h>
#include <math.h>




typedef struct veiculo{
    int id;
    double position;
    double length;
    double velocity;
    double desiredVel;
    double desiredVelOriginal;
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

double aceleracaoF(Veiculo * atual){
    double deltaVA = atual->velocity - atual->posterior->velocity;
    double S = sEstrela(atual->maxA,atual->desiredA,atual->timeSafety,atual->velocity,deltaVA,atual->timeSafety);
    //printf("ENTROU ACELERACAO");
    double potencia = atual->velocity / atual->desiredVel;
    double posPosterior = atual->posterior->position;
    double Sa = posPosterior - atual->position - atual->posterior->length;
    double potencia2 = S/Sa;
    double accel = atual->desiredA*(1 - pow(potencia,accelExp) - pow(potencia2,2));
    //printf("desiredA: %lf potencia: %lf potencia2: %lf accelExp: %lf\n",atual->desiredA,potencia,potencia2,accelExp);
    //printf("DeltaVA: %lf S: %lf Accel = %lf\n",deltaVA,S,accel);
    return accel;
}
double aceleracaoF2(Veiculo * atual,double rUM[]){
    double deltaVA = rUM[1] - atual->posterior->velocity;
    double S = sEstrela(atual->maxA,atual->desiredA,atual->timeSafety,rUM[1],deltaVA,atual->timeSafety);
    //printf("ENTROU ACELERACAO");
    double potencia = rUM[1] / atual->desiredVel;
    double posPosterior = atual->posterior->position;
    double Sa = posPosterior - rUM[0] - atual->posterior->length;
    double potencia2 = S/Sa;
    double accel = atual->desiredA*(1 - pow(potencia,accelExp) - pow(potencia2,2));
    //printf("desiredA: %lf potencia: %lf potencia2: %lf accelExp: %lf\n",atual->desiredA,potencia,potencia2,accelExp);
    //printf("DeltaVA: %lf S: %lf Accel = %lf\n",deltaVA,S,accel);
    return accel;
}
double aceleracaoF3(Veiculo * atual,double atualVelocity,double atualPosition,double posteriorVelocity,double posteriorPosition){
    double deltaVA = atualVelocity - posteriorVelocity;
    double S = sEstrela(atual->maxA,atual->desiredA,atual->timeSafety,atualVelocity,deltaVA,atual->timeSafety);
    //printf("ENTROU ACELERACAO");
    double potencia = atualVelocity / atual->desiredVel;
    double posPosterior = posteriorPosition;
    double Sa = posPosterior - atualPosition - atual->posterior->length;
    double potencia2 = S/Sa;
    double accel = atual->desiredA*(1 - pow(potencia,accelExp) - pow(potencia2,2));
    //printf("desiredA: %lf potencia: %lf potencia2: %lf accelExp: %lf\n",atual->desiredA,potencia,potencia2,accelExp);
    //printf("DeltaVA: %lf S: %lf Accel = %lf\n",deltaVA,S,accel);
    return accel;
}


double EulerPosition(Veiculo * atual, double F[], double h){
    return atual->position + h*(F[0]);
}
double EulerVelocity(Veiculo * atual, double F[], double h){
    return atual->velocity + h*(F[1]);
}
void printCar(ListaVeiculos * lista,double h){



    //if(pos > roadLength)
      // pos = pos/roadLength;
    FILE * fp;
    fp = fopen( "0.dat", "a" );
    //if(lista->first->position < roadLength)
        fprintf(fp,"%lf %lf\n",lista->first->position,h);
    //else
      //  fprintf(fp,"%lf %lf\n",lista->first->position/roadLength,h);
    fclose(fp);
    fp = fopen( "1.dat", "a" );
    //if(lista->first->position < roadLength)
        fprintf(fp,"%lf %lf\n",lista->first->posterior->position,h);
    //else
      //  fprintf(fp,"%lf %lf\n",lista->first->posterior->position/roadLength,h);
    fclose(fp);
    fp = fopen( "2.dat", "a" );
    //if(lista->first->position < roadLength)
        fprintf(fp,"%lf %lf\n",lista->first->posterior->posterior->position,h);
    //else
     //   fprintf(fp,"%lf %lf\n",lista->first->posterior->posterior->position/roadLength,h);
    fclose(fp);
}
//trecho de velocidade melhor 200-300!!!
void rungeKutta2(ListaVeiculos * lista){
    //programar euler modificado;
    int i;
    double h;
    for(h = 0;h < 0.35; h = h + 0.0001){
        //printf("%lf\n",h);
        Veiculo * atual = lista->first;
        for(i = 0; i < lista->n; i++){
            int nVoltas = atual->position/roadLength;
            if(atual->position > (200 +roadLength*nVoltas) && atual->position < (300+roadLength*nVoltas) ){
                atual->desiredVel = atual->desiredVelOriginal/2;
            } else
                atual->desiredVel = atual->desiredVelOriginal;
            //printf("Pos: %lf nVoltas: %d\n",atual->position,nVoltas);
            double accel;
            //if(atual->posterior == NULL){
            //    double potencia = atual->velocity / atual->desiredVel;
            //    accel = atual->desiredA*(1 - pow(potencia,accelExp));
            //} else{
                accel = aceleracaoF(atual);
            //}

            double F[] = {
                atual->velocity,
                accel
            };

            double rUM[] = {
                EulerPosition(atual,F,h),
                EulerVelocity(atual,F,h)
            };
            double accel2;
            //if(atual->posterior == NULL){
            //    double potencia = atual->velocity / atual->desiredVel;
            //    accel2 = atual->desiredA*(1 - pow(potencia,accelExp));
            //} else{
                accel2 = aceleracaoF2(atual,rUM);
            //}
            double F2[] = {
                rUM[1],
                accel2
            };
            double velEuler = F2[0];
            double accelEuler = F2[1];
            atual->position = atual->position + (h/2)*velEuler;
            atual->velocity = atual->velocity + (h/2)*accelEuler;
            if(atual->id == 0){
                ghost->position = ghost->position + (h/2)*velEuler;
                ghost->velocity = ghost->velocity + (h/2)*accelEuler;
            }
            //printCar(atual,h);
            atual = atual->posterior;
            //printf("***************************\n");
        }
        printCar(lista,h);
        //printf("-------------------------------\n");
        //getchar();
    }

}

//Yn+1 = Yn + h/2(F(n+1,Yn+1));


void BDF2(ListaVeiculos * lista){

    double anterior[4][2];
    double corrente[4][2];
    double futuro[4][2];
    //primeiro passo
    int i;
    double h = 0.0001;
    Veiculo * atual = lista->first;
    for(i = 0; i < lista->n; i++){
        int nVoltas = atual->position/roadLength;
        if(atual->position > (200 +roadLength*nVoltas) && atual->position < (300+roadLength*nVoltas) ){
            atual->desiredVel = atual->desiredVelOriginal/2;
        } else
            atual->desiredVel = atual->desiredVelOriginal;
        double accel;
        accel = aceleracaoF(atual);
        double F[] = {
            atual->velocity,
            accel
        };

        double rUM[] = {
            EulerPosition(atual,F,h),
            EulerVelocity(atual,F,h)
        };
        double accel2;

            accel2 = aceleracaoF2(atual,rUM);

        double F2[] = {
            rUM[1],
            accel2
        };
        double velEuler = F2[0];
        double accelEuler = F2[1];
        atual->position = atual->position + (h/2)*velEuler;
        atual->velocity = atual->velocity + (h/2)*accelEuler;
        if(atual->id == 0){
            ghost->position = ghost->position + (h/2)*velEuler;
            ghost->velocity = ghost->velocity + (h/2)*accelEuler;
            anterior[3][0] = ghost->position;
            anterior[3][1] = ghost->velocity;
        }
        anterior[i][0] = atual->position;
        anterior[i][1] = atual->velocity;
        //printf("AtualPos: %lf\n",atual->position);
        //printf("AtualVel: %lf\n",atual->velocity);
        atual = atual->posterior;
    }
    //printCar(lista,h);
    //printf("\n");
    h = h + 0.0001;
    //segundo passo
    atual = lista->first;
    for(i = 0; i < lista->n; i++){
        int nVoltas = atual->position/roadLength;
        if(atual->position > (200 +roadLength*nVoltas) && atual->position < (300+roadLength*nVoltas) ){
            atual->desiredVel = atual->desiredVelOriginal/2;
        } else
            atual->desiredVel = atual->desiredVelOriginal;
        double accel;
        accel = aceleracaoF(atual);
        double F[] = {
            atual->velocity,
            accel
        };

        double rUM[] = {
            EulerPosition(atual,F,h),
            EulerVelocity(atual,F,h)
        };
        double accel2;

            accel2 = aceleracaoF2(atual,rUM);

        double F2[] = {
            rUM[1],
            accel2
        };
        double velEuler = F2[0];
        double accelEuler = F2[1];
        atual->position = atual->position + (h/2)*velEuler;
        atual->velocity = atual->velocity + (h/2)*accelEuler;
        if(atual->id == 0){
            ghost->position = ghost->position + (h/2)*velEuler;
            ghost->velocity = ghost->velocity + (h/2)*accelEuler;
            corrente[3][0] = ghost->position;
            corrente[3][1] = ghost->velocity;
        }
        corrente[i][0] = atual->position;
        corrente[i][1] = atual->velocity;
        //printf("AtualPos: %lf\n",atual->position);
        //printf("AtualVel: %lf\n",atual->velocity);
        atual = atual->posterior;
    }
    //printCar(lista,h);
    //exit(1);
    //getchar();
    for(h = 0.0003;h < 10; h = h + 0.0001){
        atual = lista->first;
        /*printf("AnteriorPos: %lf\n",anterior[0][0]);
        printf("correntePos: %lf\n",corrente[0][0]);
        printf("AnteriorPos: %lf\n",anterior[1][0]);
        printf("correntePos: %lf\n",corrente[1][0]);
        printf("AnteriorPos: %lf\n",anterior[2][0]);
        printf("correntePos: %lf\n",corrente[2][0]);
        printf("AnteriorPos: %lf\n",anterior[3][0]);
        printf("correntePos: %lf\n",corrente[3][0]);
        getchar();*/
        //printf("\nFUTURO------------------\n");
        for(i = 0; i < lista->n; i++){
            int nVoltas = atual->position/roadLength;
            if(atual->position > (200 +roadLength*nVoltas) && atual->position < (300+roadLength*nVoltas) ){
            atual->desiredVel = atual->desiredVelOriginal/2;
            } else
                atual->desiredVel = atual->desiredVelOriginal;
            double accel;
            accel = aceleracaoF(atual);
            double F[] = {
                atual->velocity,
                accel
            };

            double rUM[] = {
                EulerPosition(atual,F,h),
                EulerVelocity(atual,F,h)
            };
            double accel2;

                accel2 = aceleracaoF2(atual,rUM);

            double F2[] = {
                rUM[1],
                accel2
            };
            double velEuler = F2[0];
            double accelEuler = F2[1];
            futuro[i][0] = atual->position + (h/2)*velEuler;
            futuro[i][1] = atual->velocity + (h/2)*accelEuler;

            //printf("Atualid: %d\n",atual->id);
            //printf("FuturoPos: %lf\n",futuro[i][0]);
            //printf("FuturoVel: %lf\n",futuro[i][1]);


            if(atual->id == 0){
                futuro[3][0] = ghost->position + (h/2)*velEuler;
                futuro[3][1] = ghost->velocity + (h/2)*accelEuler;
            }
        }
        //getchar();

        //////CALCULAR F3
        double F3[3][2];
        int c;
        atual = lista->first;
        for(c = 0; c < 3; c++){
                int nVoltas = futuro[c][0]/roadLength;
                if(futuro[c][0] > (200 +roadLength*nVoltas) && futuro[c][0] < (300+roadLength*nVoltas) ){
                    atual->desiredVel = atual->desiredVelOriginal/2;
                } else
                    atual->desiredVel = atual->desiredVelOriginal;
            F3[c][0] = futuro[c][1];
            F3[c][1] = aceleracaoF3(atual,futuro[c][1],futuro[c][0],futuro[c+1][1],futuro[c+1][0]);
            atual = atual->posterior;
        }
        //printf("\nF3------------------\n");
        /*for(c = 0; c < 3; c++){
            int j;
            for(j = 0; j < 2; j++){
                printf("%lf ",F3[c][j]);
            }
            printf("\n");
        }*/
        //getchar();
        atual = lista->first;
        //AQUI!!!!!!!!!!!!!!!!!!!!!!!!!
        //printf("\nATUAL------------------\n");
        for(i = 0; i < lista->n; i++){
            //printf("anteriorPos: %lf\nanteriorVel: %lf",anterior[i][0],anterior[i][1]);
            //printf("\ncorrentePos: %lf\ncorrenteVel: %lf\n",corrente[i][0],corrente[i][1]);
            double atualPosition = 4*corrente[i][0]/3 - anterior[i][0]/3 + 2*h*F3[i][0]/3;
            double atualVelocity = 4*corrente[i][1]/3 - anterior[i][1]/3 + 2*h*F3[i][1]/3;
            //printf("atualPosition = %lf - %lf + %lf\n",4*corrente[i][0]/3,anterior[i][0]/3,2*h*F3[i][0]/3);
            //printf("atualVelocity = %lf - %lf + %lf\n",4*corrente[i][1]/3 , anterior[i][1]/3 , 2*h*F3[i][1]/3);
            //printf("AtualPos: %lf\n",atualPosition);
            //printf("AtualVel: %lf\n",atualVelocity);
            //getchar();
            atual->position = atualPosition;
            atual->velocity = atualVelocity;
            printf("Atualid: %d\n",atual->id);
            printf("AtualPos: %lf\n",atual->position);
            printf("AtualVel: %lf\n",atual->velocity);
            if(atual->id == 0){
                ghost->position = ghost->position + F3[0][0];
                ghost->velocity = ghost->velocity + F3[0][1];
                corrente[3][0] = ghost->position;
                corrente[3][1] = ghost->velocity;
            }
            anterior[i][0] = corrente[i][0];
            anterior[i][1] = corrente[i][1];
            corrente[i][0] = atual->position;
            corrente[i][1] = atual->velocity;
            //printf("anteriorPos: %lf\nanteriorVel: %lf",anterior[i][0],anterior[i][1]);
            //printf("\ncorrentePos: %lf\ncorrenteVel: %lf\n*******************\n",corrente[i][0],corrente[i][1]);
            //getchar();
            atual = atual->posterior;
        }
        printCar(lista,h);
    }


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
    roadLength = 500;
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
        car->desiredVelOriginal = desiredVel;
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
            car->position = lista->last->position + 100;
            lista->last->posterior = car;
            lista->last = car;
            lista->n++;
        }
        //printf("car position = %lf |", car->position);
    }
    //printando a lista para teste
    lista->last->posterior = ghost;

    //printf("\nEntrou no RungeKutta!\n");
    //rungeKutta2(lista);
    BDF2(lista);


    return 0;
}
