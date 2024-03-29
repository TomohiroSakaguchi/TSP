/* traveling_salesman */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXCITY 200000

int main(int argc,char *argv[])
{
  int ncity;                    /* the total number of cities you must visit*/
  double x[MAXCITY],y[MAXCITY]; /* array for position data */
  int nroute[MAXCITY]={0},min_route[MAXCITY]={0};      /* array for route data */
  double p,T,rnd,dis_tot,def_dis=0.0,new_dis=0.0,min_dis=3000000.0;               /* total distance */

  /* local variables */
  FILE *input, *output;
  int i,j,ii,jj,nrnd,second,tmp;
  double rx,ry,rr;
  clock_t cpu_time1,cpu_time2;


  /* Output how to use this program*/
  if(argc!=2){
    printf("usage:./a.out filename\n");
    exit(1);
  }


  /* Error output (check City data name) */
  input = fopen(argv[1], "r");
  if(input==NULL){
    printf("no such file or directory\n");
    exit(1);
  }


  /* Read City data */
  fscanf(input,"%d", &ncity);
  for(i = 0; i < ncity; i++){
    fscanf(input,"%lf %lf",&x[i],&y[i]);
  }
  fclose(input);


  /* Output City data (for Check) */
  printf("ncity\n");
  printf("%d\n",ncity);
  if(ncity > MAXCITY){
    printf("ncity is too large\n");
    exit(1);
  }
  printf("Initial City Position:\n");
  for(i = 0; i < ncity; i++){
    printf("%d,%lf,%lf\n",i,x[i],y[i]);
  }


  /* Stop Watch START*/
  cpu_time1 = clock();


  /*
    ----------------------------------------
    !!
    !! Nearest Neighbor algorithm START!
    !!
    ----------------------------------------
  */
  printf("-------------------------------\n");
  printf("Route Search Algorithm START\n");
  printf("-------------------------------\n");

  int count = 0;
  /* set initial route*/
  for (i=0; i < ncity; i++){
    nroute[i] = i;
    min_route[i] = i;
  }

  /* culculate default distance*/
  for(i=0; i < ncity-1; i++){
    ii = nroute[i];
    jj = nroute[i+1];
    rx = x[ii] - x[jj];
    ry = y[ii] - y[jj];
    rr = sqrt(rx*rx + ry*ry);
    def_dis = def_dis + rr;
  }
  ii = nroute[ncity-1];
  jj = nroute[0];
  rx = x[ii] - x[jj];
  ry = y[ii] - y[jj];
  rr = sqrt(rx*rx + ry*ry);
  def_dis = def_dis + rr;
  printf("default dist.%lf\n",def_dis);
  min_dis = def_dis;

  while(1){
  /* generate random number */
  srand((unsigned)time(NULL));
  nrnd = rand()%(ncity);
  second = (nrnd + 1) % ncity;
  //printf("one sity is %d\n",nrnd);
  //printf("second sity is %d\n",second);

  /*swap city*/
  tmp = nroute[nrnd];
  nroute[nrnd] = nroute[second];
  nroute[second] = tmp;

  /* culculate swapped distance*/
  new_dis = 0.0;
  for(i=0; i < ncity-1; i++){
    ii = nroute[i];
    jj = nroute[i+1];
    //printf("%d,%lf,%lf\n",ii,x[ii],y[ii]);
    rx = x[ii] - x[jj];
    ry = y[ii] - y[jj];
    rr = sqrt(rx*rx + ry*ry);
    new_dis = new_dis + rr;
  }
  ii = nroute[ncity-1];
  jj = nroute[0];
  //printf("%d,%lf,%lf\n",ii,x[ii],y[ii]);
  rx = x[ii] - x[jj];
  ry = y[ii] - y[jj];
  rr = sqrt(rx*rx + ry*ry);
  new_dis = new_dis + rr;
  //printf("swapped dist.%lf\n",new_dis);

  if(new_dis <= min_dis){
    min_dis = new_dis;
    for(i=0 ; i < ncity ; i++){
      min_route[i] = nroute[i];
      //printf("%d\n", min_route[i]);
    }
    count++;
//    printf("これが最小やで\n");
  }
  else{
    /* random number generator */
    srand((unsigned)time(NULL)); 
    rnd = (double)rand()/RAND_MAX;
    //printf("実数の乱数%lf\n",rnd);
    T = 8.7;
    p = exp(-(new_dis - min_dis)/T);
    //printf("pの値 %lf\n",p);
    if(rnd < p ){
     //printf("悪くなったけど採用\n"); 
     continue;
     //printf("continue確認\n");
    }else{
      /*swap city*/
      tmp = nroute[nrnd];
      nroute[nrnd] = nroute[second];
      nroute[second] = tmp;
      //printf("悪くなったので却下\n");
    }  
  }
  if(count==10000000){
    break;
  }

  }
  printf("-------------------------------\n");
  printf("Route Search Algorithm END\n");
  printf("-------------------------------\n");

  /*
    !!---------------------------------------
    !!
    !! Nearest Neighbor algorithm END!
    !!
    !!---------------------------------------
  */

  /* Stop Watch END */
  cpu_time2 = clock();
  printf("minimize distance is %20.14lf\n",min_dis);
  printf("temperture is %lf\n",T);
  /* Output City Data */
  printf("Optimized Route\n");
  for (i=0 ; i < ncity ; i++){
    j= min_route[i];
    printf("%d,%lf,%lf\n",j,x[j],y[j]);
  }


  /* Output Distance */
  printf("\n");
  printf("Total Distance!\n");
  dis_tot = 0.0;
  for(i=0; i < ncity-1; i++){
    ii = min_route[i];
    jj = min_route[i+1];
    rx = x[ii] - x[jj];
    ry = y[ii] - y[jj];
    rr = sqrt(rx*rx + ry*ry);
    dis_tot = dis_tot + rr;
  }
  ii = min_route[ncity-1];
  jj = min_route[0];
  rx = x[ii] - x[jj];
  ry = y[ii] - y[jj];
  rr = sqrt(rx*rx + ry*ry);
  dis_tot = dis_tot + rr;
  printf("%lf\n",dis_tot);


  /* Output GNUPLOT data */
  output = fopen("route.dat","w");
  fprintf(output,"%lf\n",dis_tot);
  fprintf(output,"%20.14f\n",(double)(cpu_time2-cpu_time1)/(double)CLOCKS_PER_SEC);
  for(i = 0; i < ncity; i++){
    j = min_route[i];
    //fprintf(output,"%d %lf %lf\n",j,x[j],y[j]);
  }
  j = min_route[0];
  //fprintf(output,"%d %lf %lf\n",j,x[j],y[j]);
  fclose(output);


  /* Output Computational Time */
  printf("\n");
  printf("Elapsed Time: %20.14f sec\n",(double)(cpu_time2-cpu_time1)/(double)CLOCKS_PER_SEC);

  return 0;
}
