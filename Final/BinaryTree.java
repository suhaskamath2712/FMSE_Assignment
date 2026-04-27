package BinaryTree;
import gov.nasa.jpf.symbc.Debug;
import gov.nasa.jpf.symbc.Symbolic;

public class BinaryTree {
    int elem;
    BinaryTree left, right;
  
    public static void checkSorted(BinaryTree root) {
      if (root == null)
        return;
      if (root.left != null) {
        if (root.left.elem > root.elem) 
	      assert(false);
      }    
      if (root.right != null) {
        if (root.right.elem < root.elem)
	      assert(false);
      }
    }

    public static void main(String[] args) {	
		BinaryTree X = new BinaryTree();
		checkSorted(X);
   }
}
