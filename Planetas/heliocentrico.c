//Empiezo de nuevo 
//CODIGO PLANETAS QUE POR AHORA FUNCIONA MEJOR
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define Mt 1.99e30
#define G 6.67e-11
#define c 1.496e11
#define n 5
#define h 0.1

void periodo(double T[], double ry[], double t, double aux[], int contador[]){
    int i;
    for (i=0; i<n; i++){
    if(aux[i]>=0 && ry[i]<0){
    T[i]=t;
    contador[i]++;
    }
        
    }
}

void energia (double vx[], double vy[], double rx[], double ry[], double m[], double E){
    double suma, modulo;
    int i,j;
     //Inicializo la energía
    for (i=0;i<n;i++){
        suma=0; //Inicializo la variable suma
        for (j=0;j<n;j++){
            if(j!=i){ //para que no se anule el denominador
                modulo=(pow(pow(rx[i]-rx[j],2)+pow(ry[i]-ry[j],2),0.5)); //Calculo el modulo de la distancia con cada planeta
                suma+=m[j]/modulo;
            }
       
        
    }
   // printf("%lf\n",suma);

    E+=0.5*m[i]*(pow(vx[i],2)+pow(vy[i],2)) - G*m[i]*suma;
    }
   // printf("\n");
    printf("%lf\n",E);
   // printf("\n");
    
}

void  aceleracion(double ax[], double ay[], double rx[], double ry[], double m[]){
    int i, k;
    double sum_x, sum_y, dif_x, dif_y, mod;
    for (i = 0; i <n; i++)  // para el sol y cada planeta
    {
        sum_x = 0;  // inicializo ambas coordenadas a 0
        sum_y = 0;
        for (k = 0; k < n; k++)  // empiezo en k=0 (sol) y para todos los planetas
        {
            if (k != i)  // si el planeta k es distinto al i (distinto al considerado para la aceleracion)
            {
                dif_x = rx[i] - rx[k];  // componente x vector diferencia
                dif_y = ry[i] - ry[k];  // componente y vector diferencia
                mod = sqrt(pow(dif_x, 2) + pow(dif_y, 2));  // hago el modulo del vector diferencia
                sum_x += dif_x * m[k] / (pow(mod, 3));  // ecuacion movimiento componente x
                sum_y += dif_y * m[k] / (pow(mod, 3));  // ecuacion movimiento componente y
            }
        }
        ax[i] = -sum_x;  // componente x aceleracion del planeta i
        ay[i] = -sum_y;  // componente y aceleracion del planeta i
    }
}


void posiciones (double ax[], double ay[], double rx[], double ry[], double vx[], double vy[]){
    int j;
    for (j = 0; j <n; j++)  // para el sol y cada planeta
    {
        rx[j]=rx[j]+h*vx[j]+pow(h,2)/2*ax[j];
        ry[j]=ry[j]+h*vy[j]+pow(h,2)/2*ay[j];
    }
}

void pesos (double ax[], double ay[], double wx[], double wy[], double vx[], double vy[]){
    int j;
    for (j = 0; j <n; j++)  // para el sol y cada planeta
    {
        wx[j]=vx[j]+h/2*ax[j];
        wy[j]=vy[j]+h/2*ay[j];
    }
}

void velocidad (double ax[], double ay[], double wx[], double wy[], double vx[], double vy[]){
    int j;
    for (j = 0; j <n; j++)  // para el sol y cada planeta
    {   
        vx[j]=0.0;
        vy[j]=0.0;
        vx[j]=wx[j]+h/2*ax[j];
        vy[j]=wy[j]+h/2*ay[j];
    }
}

int main(){
    //PREVIO: Extraigo las condiciones inciales de masas de un fichero.
    double m[n];
    int i;
    // abro fichero masas
    FILE *fichmasa;
    fichmasa = fopen("masa.txt", "r");
    for (i = 0; i<n; i++)
    {
        fscanf(fichmasa, "%lf", &m[i]); // leo el fichero con las masas en kg y guardo las masas en el vector
    }
    //cierro fichero
    fclose(fichmasa);

    //Compruebo que las masas se han volcado bien en el vector
    // for (i = 0; i<n; i++)
   // {
     //   printf( "%lf\n", m[i]);
  //  }
    
    //Reescalo las masas y lo compruebo
    for (i = 0; i<n; i++){
        m[i]/=Mt; //Lo he reescalado con la masa de la tierra porque con la del sol se me iban a cero.
        //printf( "%lf\n", m[i]);
    }

    //PASO 0: Declaro h y t, inicializo t, posiciones iniciales y velocidades iniciales.
    //double h;
    //h=0.1;
    //para el tiempo tendré el tiempo total de simulación y el tiempo como contador
    double t;
    double ts;
    ts=100;
    //Inicializo el contador
    t=0;
    //Declaro el vector posicion y lo relleno con los datos iniciales
    double rx[n];
    double ry[n];
    FILE *fichpos;
    fichpos = fopen("posiciones.txt", "r");
    for (i = 0; i<n; i++)
    {
        fscanf(fichpos, "%lf\t%lf", &rx[i], &ry[i]); // leo el fichero con las masas en kg y guardo las masas en el vector
    }
    //cierro fichero
    fclose(fichpos);

   //Compruebo que lo ha leido bien
   // for (i = 0; i<n; i++)
    //{
       // printf("%lf\t%lf\n", rx[i], ry[i]); // leo el fichero con las masas en kg y guardo las masas en el vector
    //}
     
    //}

    //Los lee bien
    //Ahora reescalo
    for (i = 0; i<n; i++)
    {
        rx[i]/=c;
        ry[i]/=c;
        //printf("%lf\t%lf\n", rx[i], ry[i]);

    }

    //Hago lo mismo con las velocidades
    double vx[n];
    double vy[n];
    FILE *fichvel;
    fichvel = fopen("velocidades.txt", "r");
    for (i = 0; i<n; i++)
    {
        fscanf(fichvel, "%lf\t%lf", &vx[i], &vy[i]); // leo el fichero con las masas en km/s y guardo las masas en el vector
    }
    //cierro fichero
    fclose(fichvel);


     //Compruebo que lo ha leido bien
   //for (i = 0; i<n; i++)
   // {
    //    printf("%lf\t%lf\n", vx[i], vy[i]); 
    //}

     //Ahora reescalo
    for (i = 0; i<n; i++)
    {
        vx[i]=pow(c,0.5)/pow(G*Mt,0.5)*vx[i];
        vy[i]=pow(c,0.5)/pow(G*Mt,0.5)*vy[i];
        //printf("%lf\t%lf\n", vx[i], vy[i]);

    }
     //BIEN REESCALADO, UNIDADES COHERENTES


    //PASO 1: Evaluo las aceleraciones a partir de los datos reescalados y la ecuacion del movimiento
    //IMPORTANTE: Las aceleraciones solo dependen de las posiciones relativas
    //Para esto voy a crear una funcion a la que voy a llamar y que luego inlcuire en el ciclo while de tiempo

    //Declaro las aceleraciones

    double ax[n];
    double ay[n];
    aceleracion(ax, ay, rx, ry, m);

    // for (i = 0; i<n; i++)
    //{
       
      //  printf("%lf\t%lf\n", ax[i], ay[i]);

    //}

    //PASO 2: Comienzo el ciclo while y calculo las posiciones en t+h y los pesos
    double wx[n];
    double wy[n];

    //Abro el fichero donde voy a escribir luego
    FILE *fichdata;
    fichdata = fopen("planets_data.dat", "w");

    FILE *fichdateng;
    fichdateng = fopen("energia.dat", "w");

    FILE *fichdatper;
    fichdatper = fopen("periodo.dat", "w");


   //Declaro la energia y el tiempo 

   double E;
   double T[n];
   int contador [n];
   double aux[n];
   //Inicializo los periodos
   for (i=0; i<n; i++) contador[i]=0;

    int p;
    while(t<=ts){
      
        energia (vx, vy, rx, ry, m, E);
        //Escribo la energia en su fichero para luego representarla
        fprintf(fichdateng, "%lf\t%lf\n", t, E);

        //Llamo a la funcion de posicion y a la de pesos
        posiciones (ax,ay,rx,ry,vx,vy);
        periodo(T, ry, t, aux, contador);
        for (i=0;i<n;i++){
            aux[i]=ry[i];
        }
        pesos (ax,ay,wx,wy,vx,vy);
        //Vuelco los datos de posicion en un fichero  
        for (p = 0; p< n; p++)
        {
            fprintf(fichdata, "%.5lf, %.5lf\n", rx[p], ry[p]);
        }
        fprintf(fichdata, "\n"); // para separar los bloques de tiempo en el fichero


        //PASO 3: Evaluo la aceleración usando las nuevas posiciones
       aceleracion(ax, ay, rx, ry, m);

       //PASO 4: Evaluo las velocidades
       velocidad (ax,ay,wx,wy,vx,vy);

       //PASO 5: Avanza el contador
       t=t+h;
    }

    //cierro ficheros
    fclose(fichdata);
    fclose(fichdateng);

     // Escribo el periodo en su fichero
        for (p = 0; p< n; p++)
        {
            
            fprintf(fichdatper, "%.5lf\n", T[p]/(contador[p]*1.0));
        
        }
        fprintf(fichdatper, "\n");
     fclose(fichdatper);
    
    }

    


