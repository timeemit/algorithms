#include <stdio.h>
#include <stdlib.h>

struct SortedCount {
  unsigned long int inversion_count;
  unsigned long int length;
  unsigned long int * numbers;
};

void print_sorted_count ( struct SortedCount *input ) {
  for ( int i = 0; i < input->length; i++ ) { printf("%lu, ", input->numbers[i]); }
}; 

void report_sorted_count( struct SortedCount *input ) {
  printf("\n\tInversion Count: %lu\n\tLength: %lu\n\tNumbers as: ", input->inversion_count, input->length);
  print_sorted_count(input);
  printf("\n\n");
};

struct SortedCount merge_and_split_count_func( struct SortedCount * left, struct SortedCount * right ){
  struct SortedCount sorted_count = {
    .length = left->length + right->length,
    .inversion_count = 0
  };
  int left_index = 0;
  int right_index = 0;
  int step_count = 0;

  sorted_count.numbers = malloc(sorted_count.length * sizeof(int));
  printf("\nLeft: ");
  print_sorted_count(left);
  printf("\nRight: ");
  print_sorted_count(right);
  while ( left_index < left->length && right_index < right->length ) {

    printf("\n\t%d: %lu (%d) %lu (%d) := %lu", step_count, left->numbers[left_index], left_index, right->numbers[right_index], right_index, sorted_count.inversion_count);
    if ( left->numbers[left_index] < right->numbers[right_index] ) {
      if ( left_index < left->length ) {
        sorted_count.numbers[step_count] = left->numbers[left_index];
        left_index++;
        if ( left_index == left->length ) {
          // No numbers remaining in left.  Copy over all numbers in right.
          for ( int i = right_index; i < right->length; i++ ) {
            printf("\n\t\tRight Copy. step_count: %d -- right: %lu (%d)", step_count, right->numbers[i], i);
            step_count++;
            sorted_count.numbers[step_count] = right->numbers[i];
          }
        }
      } else {
        sorted_count.numbers[step_count] = right->numbers[right_index];
        right_index++;
      }
    } else {
      sorted_count.inversion_count += left->length - left_index;
      sorted_count.numbers[step_count] = right->numbers[right_index];
      if ( right_index < right->length - 1 ) {
        right_index++;
      } else {
        for ( int i = left_index; i < left->length; i++ ) {
          // No numbers remaining in right.  Copy over all numbers in left.
          printf("\n\t\tLeft Copy. step_count: %d -- left: %lu (%d)", step_count, left->numbers[i], i);
          step_count++;
          sorted_count.numbers[step_count] = left->numbers[i];
        }
        return sorted_count;
      }
    }

    step_count++;
  }
  return sorted_count;
};

struct SortedCount count( struct SortedCount * input ){
  struct SortedCount left, *left_ptr;
  struct SortedCount right, *right_ptr;
  struct SortedCount split;
  struct SortedCount basecase;

  *left_ptr = left;
  *right_ptr = right;

  if ( input->length == 1 ) {
    basecase.inversion_count = 0;
    basecase.length = 1;
    basecase.numbers = malloc(1 * sizeof(int));
    basecase.numbers[0] = input->numbers[0];
    return basecase;
  } else {
    // Left
    left.length = input->length / 2;
    left.numbers = malloc(left.length * sizeof(int));
    left.inversion_count = 0;
    for ( int i = 0; i < left.length; i++ ) { left.numbers[i] = input->numbers[i]; }
    printf("Left Initialized:");
    report_sorted_count(&left);

    // Count Left
    left = count( &left );
    printf("Left Reports");
    report_sorted_count(&left);

    // Right
    right.length = ( input->length / 3 ) + ( input->length % 2 );
    right.numbers = malloc(right.length * sizeof(int));
    right.inversion_count = 0;
    for ( int i = 0; i < right.length; i++ ) { right.numbers[i] = input->numbers[i + left.length]; }
    printf("Right Initialized:");
    report_sorted_count(&right);
    
    // Count Right
    right = count( &right );
    printf("Right Reports");
    report_sorted_count(&right);

    // Split
    printf("Splitting");
    split = merge_and_split_count_func( left_ptr, right_ptr );
    report_sorted_count(&split);

    // Free
    free(left.numbers);
    free(right.numbers);

    // Merge
    split.inversion_count += left.inversion_count + right.inversion_count;
    return split;
  }

};

int main( int argc, char *argv[] ) {
  FILE* read_file;
  unsigned long int number_of_numbers = atoi( argv[2] );
  unsigned long int all_numbers[number_of_numbers];
  struct SortedCount total_split_inversions;

  read_file = fopen(argv[1], "r");

  // Read file
  for ( int i = 0; i < number_of_numbers; i++ ) {
    fscanf(read_file, "%lu", &all_numbers[i]);
    printf("%lu\n", all_numbers[i]);
  }

  // Initialize
  total_split_inversions.numbers = malloc(number_of_numbers * sizeof(int));
  total_split_inversions.length = number_of_numbers;
  total_split_inversions.inversion_count = 0;
  for ( int i = 0; i < number_of_numbers; i++ ) { total_split_inversions.numbers[i] = all_numbers[i]; }

  // Calculate
  total_split_inversions = count(&total_split_inversions);

  // Output
  printf("Count: %lu\n", total_split_inversions.inversion_count);
  printf("Sorted: ");
  print_sorted_count(&total_split_inversions);
  
  fclose(read_file);
  return 0;
}
