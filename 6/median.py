import heapq

unheap = [ int(line) for line in open("median.txt") ]
heap = []
medians = []

for i in range(len(unheap)):
    heapq.heappush(heap, unheap[i])
    heap.sort()
    if i % 2:
        # i is odd, so len(heap) == i + 1 is even
        # print("odd")
        median_index = int((i + 1) / 2) - 1
    else:
        # print("even")
        # i is even, so len(heap) == i + 1 is odd
        median_index = int(i / 2 + 1) - 1
    # print("median_index: {}".format(median_index))
    # print("heap: {}".format(heap))
    # print("new median: {}".format(heap[median_index]))
    medians.append(heap[median_index])

# print(medians)
print(len(medians))
print(sum(medians) % len(medians))
