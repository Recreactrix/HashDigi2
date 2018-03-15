#include <stdio.h>
#include <stdint.h>
int main(){
int x=3;
int y=64;
  	if(x<0){
		    x = x*-1;
  		  printf("%d",(y-(x%y))%y );}
            
    else{
    	  printf("%d", (x%y));
        }
}

