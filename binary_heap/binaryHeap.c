/*  
 * @file 	
 * @brief	
 * @details 
 * @author   purefarmer <464224357@qq.com> 
 * @date     
 * @par Copyright (c): purefarmer 
 */
#include "binaryHeap.h"


/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */
static void maxHeapify(int array[], uint32_t size, uint32_t index)
{
	uint32_t l , r, max;

	l = index * 2 + 1;
	r = index * 2 + 2;
	max = index;
	if (l < size && array[l] > array[max])	
	{
		max = l;	
	}
	if (r < size && array[r] > array[max])
	{
		max = r;
	}
	if (max != index)
	{
		int temp = array[max];
		array[max] = array[index];
		array[index] = temp;
		maxHeapify(array, size, max);
	}
}

/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */
static int buidlMaxHeap(int array[], uint32_t size)
{
	if (size < 1)
	{
		return 1;
	}
	uint32_t idx, start = (size - 1) / 2;

	for(idx = start;idx > 0;idx--)
	{
		maxHeapify(array, size, idx);		
	}
	maxHeapify(array, size, idx);		

	return 0;
}
/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */
int heapSort(int array[], uint32_t size)
{
	int retVal;	
	int max;
	uint32_t idx;

	retVal = buidlMaxHeap(array, size);
	if (retVal != 0) {
		return 1;
	}
	for (idx = size - 1;idx > 0;idx--) {
		max = array[idx];
		array[idx] = array[0];
		array[0] = max;
		maxHeapify(array, idx, 0);
	}
	
	return 0;
}

/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */

/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */

/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */

/*  
 * @brief        
 * @param[out]   
 * @param[in]   
 * @return       
 * @see         
 * @note        
 */
