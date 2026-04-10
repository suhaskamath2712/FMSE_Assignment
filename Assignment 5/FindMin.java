import gov.nasa.jpf.symbc.Debug;

public class FindMin {
    public static int findMin(int[] arr)
    {
        int min = arr[0];

        for (int i = 1; i < arr.length; i++)
            if (arr[i] < min)
                min = arr[i];

        return min;
    }

    public static void main(String[] args) {
        int[] ar = new int[2];
        
        ar[0] = Debug.makeSymbolicInteger("m0");
        ar[1] = Debug.makeSymbolicInteger("m1");

        int r = findMin(ar);

        // assertion
        // write code here that will check if r is the minimum
        // value in array ‘ar’ and invoke ‘assert false’ otherwise.
        boolean lessThanOrEqualAll = true;

        for (int i = 0; lessThanOrEqualAll && i < ar.length; i++)
            lessThanOrEqualAll = (r <= ar[i]);

        assert (lessThanOrEqualAll);
    }
}