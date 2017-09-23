function main(){
  var i;
  i = 1;
  printf(" n    n!\n");
  while(i <= 20) {
    printf("%2d    %d\n",i,fact(i));
    i = i + 1;
  }
  return 0;
}

function fact(n){
  var i;
  i = 1;
  var factorial;
  factorial = 1;
  while(i <= n){
    factorial = factorial * i;
    i = i + 1;
  }
  return factorial;
}