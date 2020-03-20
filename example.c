#define OCR_SIMPLY_NAMES
#include "objecitifier.h"
#include <stdio.h>

class(printer) {
  char *text;
  method(void, print);
};

void printer_print(printer *this) {
  printf("Printing... Printed: %s\n", this->text);
}
void printer_constructor(printer *this) {
  memberize(this, printer, print);
}
void printer_destructor() {}

int main() {
  printer* my_printer = init_dyn(printer);

  my_printer->text = "Hey!!";
  call(my_printer, print);

  return 0;
}
