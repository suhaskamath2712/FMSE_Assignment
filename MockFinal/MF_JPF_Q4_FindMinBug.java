package MockFinal;

import gov.nasa.jpf.symbc.Debug;

public class MF_JPF_Q4_FindMinBug {

    // Intentionally buggy: it never considers the last array element.
    public static int findMinBug(int[] arr) {
        int min = arr[0];

        for (int i = 1; i < arr.length - 1; i++) { // BUG: should be i < arr.length
            if (arr[i] < min)
                min = arr[i];
        }

        return min;
    }

    public static void main(String[] args) {
        int[] a = new int[3];

        a[0] = Debug.makeSymbolicInteger("a0");
        a[1] = Debug.makeSymbolicInteger("a1");
        a[2] = Debug.makeSymbolicInteger("a2");

        int r = findMinBug(a);

        // TODO: Add an assertion that r is the minimum value in array `a`.
        // Hint: assert that r <= a[i] for all i.
        // If the property does not hold, trigger an assertion failure.

        for (int i = 0; i < a.length; i++)
            assert r <= a[i];
    }
}
