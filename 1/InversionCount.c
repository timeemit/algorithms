#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SortedCount {
  unsigned long int inversion_count;
  unsigned long int length;
  unsigned long int * numbers;
  unsigned long int * sorted;
};

void print_sorted_count ( struct SortedCount *input, int should_print_sorted ) {
  for ( int i = 0; i < input->length; i++ ) {
    if ( should_print_sorted ) {
      printf("%lu, ", input->sorted[i]); 
    } else {
      printf("%lu, ", input->numbers[i]); 
    }
  }
}; 

void report_sorted_count( struct SortedCount *input ) {
  printf("\n\tInversion Count: %lu\n\tLength: %lu\n\tNumbers as: ", input->inversion_count, input->length);
  print_sorted_count(input, 0);
  printf("\n\tSorted as: ");
  print_sorted_count(input, 1);
  printf("\n\n");
};

void merge_and_split_count( struct SortedCount * left, struct SortedCount * right, struct SortedCount * sorted_count ){
  int left_index = 0;
  int right_index = 0;
  int step_count = 0;

  sorted_count->length = left->length + right->length,
  sorted_count->inversion_count = 0;

  sorted_count->sorted = malloc( (sorted_count->length + 4) * sizeof(int));
  printf("\nLeft: ");
  print_sorted_count(left, 0);
  printf("\nRight: ");
  print_sorted_count(right, 0);
  while ( left_index < left->length && right_index < right->length ) {

    printf("\n\t%d: %lu (%d) %lu (%d) := %lu", step_count, left->sorted[left_index], left_index, right->sorted[right_index], right_index, sorted_count->inversion_count);
    if ( left->sorted[left_index] < right->sorted[right_index] ) {
      sorted_count->sorted[step_count] = left->sorted[left_index];
      left_index++;
      if ( left_index == left->length ) {
        // No numbers remaining in left.  Copy over all numbers in right.
        for ( int i = right_index; i < right->length; i++ ) {
          printf("\n\t\tRight Copy. step_count: %d -- right: %lu (%d)", step_count, right->sorted[i], i);
          step_count++;
          sorted_count->sorted[step_count] = right->sorted[i];
        }
      }
    } else {
      sorted_count->inversion_count += left->length - left_index;
      sorted_count->sorted[step_count] = right->sorted[right_index];
      if ( right_index < right->length - 1 ) {
        right_index++;
      } else {
        for ( int i = left_index; i < left->length; i++ ) {
          // No numbers remaining in right.  Copy over all numbers in left.
          printf("\n\t\tLeft Copy. step_count: %d -- left: %lu (%d)", step_count, left->sorted[i], i);
          step_count++;
          sorted_count->sorted[step_count] = left->sorted[i];
        }
        return;
      }
    }

    step_count++;
  }
};

void count( struct SortedCount * input ){
  struct SortedCount left;
  struct SortedCount right;
  struct SortedCount basecase;

  if ( input->length == 1 ) {
    return;
  } else {
    // Left
    left.length = input->length / 2;
    left.numbers = malloc( (4 + left.length) * sizeof(int));
    left.sorted = malloc( (4 + left.length) * sizeof(int));
    
    left.inversion_count = 0;
    // for ( int i = 0; i < left.length; i++ ) { 
    //   left.numbers[i] = input->numbers[i];
    //   left.sorted[i] = input->numbers[i];
    // }
    memcpy( left.numbers, input->numbers, left.length * sizeof(unsigned long int));
    memcpy( left.sorted, input->sorted, left.length * sizeof(unsigned long int));
    // printf("Left Initialized:");
    report_sorted_count(&left);

    // Count Left
    count( &left );
    printf("Left Reports");
    report_sorted_count(&left);

    // Right
    right.length = ( input->length / 2 ) + ( input->length % 2 );
    right.numbers = malloc( (4 + right.length) * sizeof(int));
    right.sorted = malloc( (4 + right.length) * sizeof(int));
    right.inversion_count = 0;
    // for ( int i = 0; i < right.length; i++ ) {
    //   right.numbers[i] = input->numbers[i + left.length]; 
    //   right.sorted[i] = right.numbers[i];
    // }
    memcpy( right.numbers, &input->numbers[left.length], right.length * sizeof(unsigned long int));
    memcpy( right.sorted, &input->sorted[left.length], right.length * sizeof(unsigned long int));
    // printf("Right Initialized:");
    report_sorted_count(&right);
    
    // Count Right
    count( &right );
    printf("Right Reports");
    report_sorted_count(&right);

    // Split
    printf("Splitting");
    merge_and_split_count( &left, &right, input );
    report_sorted_count(input);

    // Free
    free(left.numbers);
    free(right.numbers);

    // Merge
    input->inversion_count += left.inversion_count + right.inversion_count;
    return;
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
  total_split_inversions.numbers = malloc( (4 + number_of_numbers) * sizeof(int));
  total_split_inversions.sorted = malloc( (4 + number_of_numbers) * sizeof(int));
  total_split_inversions.length = number_of_numbers;
  total_split_inversions.inversion_count = 0;
  for ( int i = 0; i < number_of_numbers; i++ ) { total_split_inversions.numbers[i] = all_numbers[i]; }

  // Calculate
  count(&total_split_inversions);

  // Output
  printf("Count: %lu\n", total_split_inversions.inversion_count);
  printf("Sorted: ");
  print_sorted_count(&total_split_inversions, 1);
  
  fclose(read_file);
  return 0;
}
