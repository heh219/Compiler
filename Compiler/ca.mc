function main(){
  var lastday, month, year;
  year = 2017;
  month = 4;
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    lastday = 30;
  } 
  else { 
    if (month != 2) {
      lastday = 31;
    }
    else {
      if (leapyear(year)) {
        lastday = 29;
      }
      else {
        lastday = 28;
      }
    }
  }
}
