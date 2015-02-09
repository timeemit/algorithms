#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Array {
  unsigned long int comparison_count;
  unsigned int * numbers;
  unsigned int length;
};

void print_array ( struct Array *input ) {
  for ( int i = 0; i < input->length; i++ ) {
    //printf("%u, ", input->numbers[i]); 
  }
} 

void report_array( struct Array *input ) {
  //printf("\n\tComparison Count: %lu\n\t[", input->comparison_count);
  print_array(input);
  //printf("]\n\n");
}

unsigned int choosePivotLeft( struct Array * input, unsigned int first_index, unsigned int last_index) { 
  return first_index;
}

unsigned int choosePivotRight( struct Array * input, unsigned int first_index, unsigned int last_index) { 
  return last_index;
}

unsigned int choosePivotMedian( struct Array * input, unsigned int first_index, unsigned int last_index) { 
  unsigned int length = last_index - first_index + 1;
  unsigned int middle_index = 0;
  unsigned int middle = 0;
  unsigned int first = 0;
  unsigned int last = 0;
  if ( length <= 2 ) return first_index;
  else {
    middle_index = first_index + length / 2;
    if ( length % 2 == 0 ) middle_index -= 1;
    middle = input->numbers[middle_index];
    first = input->numbers[first_index];
    last = input->numbers[last_index];
    //printf(
    //    "\tfirst: %u (%u)\n\tmiddle %u (%u)\n\tlast %u (%u)\n\n",
    //    first, first_index, middle, middle_index, last, last_index
    //);
    if ( first < middle ) {
      if ( middle < last ) {
        return middle_index;
      } else { 
        return first < last ? last_index : first_index;
      }
    } else {
      if ( first < last ) {
        return first_index;
      } else {
        return middle < last ? last_index : middle_index;
      }
    }
  }
}

unsigned int partition ( struct Array * input, unsigned int first_index, unsigned int last_index, unsigned int pivot_index ) {
  // partition_index points to the first element of the partition numbers greater than the pivot
  unsigned int partition_index = first_index + 1;
  unsigned int swap_temp = 0;

  // Swap pivot into the place
  if ( first_index != pivot_index ){ 
    swap_temp = input->numbers[pivot_index];
    input->numbers[pivot_index] = input->numbers[first_index];
    input->numbers[first_index] = swap_temp;
  }

  // Looping
  for ( unsigned int cursor = partition_index; cursor <= last_index; cursor++ ) {
    ////printf("\tpartition value (index): %u (%u)\n", input->numbers[partition_index], partition_index);
    ////printf("\tcursor value (index): %u (%u)\n\t", input->numbers[cursor], cursor);
    //print_array(input);
    ////printf("\n\n");
    if ( input->numbers[cursor] < input->numbers[first_index] ) {
      swap_temp = input->numbers[cursor];
      input->numbers[cursor] = input->numbers[partition_index];
      input->numbers[partition_index++] = swap_temp;
    }
  }

  // Swap pivot into place
  swap_temp = input->numbers[partition_index - 1];
  input->numbers[partition_index - 1] = input->numbers[first_index];
  input->numbers[first_index] = swap_temp;

  return partition_index - 1;
}

void sort(
  struct Array * input,
  unsigned int first_index,
  unsigned int last_index,
  unsigned int ( * pivot_func ) ( struct Array *, unsigned int, unsigned int ) 
) {
  unsigned int pivot_index = 0;
  unsigned int pivot_index_after_partition = 0;

  //printf("Invocation starting at %u ending at %u", first_index, last_index);
  report_array(input);

  if ( first_index == last_index ) {
    return;
  } else {
    // Increment comparison count
    input->comparison_count += last_index - first_index;

    // Choose Pivot
    pivot_index = pivot_func( input, first_index, last_index );
    //printf("\tPivot chosen: \t%u (%u)\n\n", input->numbers[pivot_index], pivot_index);

    // Partition
    pivot_index_after_partition = partition( input, first_index, last_index, pivot_index );
    //printf("\tPartitioned with %u (%u) into\t\t", input->numbers[pivot_index_after_partition], pivot_index_after_partition);
    print_array(input);
    // Sort left partition 
    //printf("\n\nSorting Left\n");
    if ( pivot_index_after_partition != first_index ) sort( input, first_index, pivot_index_after_partition - 1, pivot_func );
    // Sort right partition
    //printf("\n\nSorting Right\n");
    if ( pivot_index_after_partition != last_index ) sort( input, pivot_index_after_partition + 1, last_index, pivot_func );

    //printf("\n\n");

    return;
  }
}

int main( int argc, char *argv[] ) {
  FILE* read_file;
  unsigned int number_of_numbers = atoi( argv[2] );
  struct Array sortedLeft;
  struct Array sortedRight;
  struct Array sortedMedian;
  unsigned int (*pivotFuncLeft) ( struct Array *, unsigned int, unsigned int )= NULL;
  unsigned int (*pivotFuncRight) ( struct Array *, unsigned int, unsigned int )= NULL;
  unsigned int (*pivotFuncMedian) ( struct Array *, unsigned int, unsigned int )= NULL;

  pivotFuncLeft = &choosePivotLeft;
  pivotFuncRight = &choosePivotRight;
  pivotFuncMedian = &choosePivotMedian;

  sortedLeft.numbers = malloc( number_of_numbers * sizeof(unsigned int));
  sortedRight.numbers = malloc( number_of_numbers * sizeof(unsigned int));
  sortedMedian.numbers = malloc( number_of_numbers * sizeof(unsigned int));

  sortedLeft.comparison_count = sortedRight.comparison_count = sortedMedian.comparison_count = 0;
  sortedLeft.length = sortedRight.length = sortedMedian.length = number_of_numbers;

  // Read file
  read_file = fopen(argv[1], "r");
  for ( int i = 0; i < number_of_numbers; i++ ) {
    fscanf(read_file, "%u", &sortedLeft.numbers[i]);
    //printf("%u\n", sortedLeft.numbers[i]);
    sortedRight.numbers[i] = sortedMedian.numbers[i] = sortedLeft.numbers[i];
  }
  
  fclose(read_file);

  // Calculate
  sort(&sortedLeft, 0, number_of_numbers - 1, pivotFuncLeft);
  sort(&sortedRight, 0, number_of_numbers - 1, pivotFuncRight);
  sort(&sortedMedian, 0, number_of_numbers - 1, pivotFuncMedian);

  // Output
  printf("Left: %lu\nRight: %lu\nMedian: %lu\n", sortedLeft.comparison_count, sortedRight.comparison_count, sortedMedian.comparison_count);

  return 0;
}
