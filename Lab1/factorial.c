#include <stdio.h>
int main(){
int number=4;
printf("Factorial of %d is: %d", number, factorial(number));
}

int factorial(int number){
int i, result = 1;
if (number==0)
	return 0;
else {
	for(i=1;i<=number;i++){
		result=result*i;
	}
}
return result;
}

