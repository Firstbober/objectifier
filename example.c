#define OCR_SIMPLY_NAMES
#include "objecitifier.h"
#include <stdio.h>

class(printer) {
  char *text;
  method(void, print);
  method(printer *, another_printer);
};

void printer_destructor() { printf("Deleted printer!\n"); }
void printer_print(printer *this) {
  printf("Printing... Printed: %s\n", this->text);
}
printer *printer_another_printer(printer *this) {
  printer *new_printer = init_dyn(printer);
  return(new_printer);
}
void printer_constructor(printer *this) {
  memberize(this, printer, print);
  memberize(this, printer, another_printer);
}

int main() {
  printer *my_printer = init_dyn(printer);

  my_printer->text = "Hey!!";
  call(my_printer, print);
  call(my_printer, another_printer);

  return(0);
}
