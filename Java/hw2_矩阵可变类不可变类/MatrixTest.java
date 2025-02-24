// package hw2;

// Mutable matrix class
class MutableMatrix {
    private double[][] data;

    // Ctor
    public MutableMatrix(double[][] data) {
        this.data = data;
    }

    // add
    public MutableMatrix add(MutableMatrix matrix) {
        if (this.data.length != matrix.data.length || this.data[0].length != matrix.data[0].length) {
            throw new IllegalArgumentException("\33[31mMatrices must have the same dimensions.\33[0m");
        }
        for (int i = 0; i < this.data.length; i++) {
            for (int j = 0; j < this.data[i].length; j++) {
                this.data[i][j] += matrix.data[i][j];
            }
        }
        return this;
    }
    // subtract
    public MutableMatrix subtract(MutableMatrix matrix) {
        if (this.data.length != matrix.data.length || this.data[0].length != matrix.data[0].length) {
            throw new IllegalArgumentException("\33[31mMatrices must have the same dimensions.\33[0m");
        }
        for (int i = 0; i < this.data.length; i++) {
            for (int j = 0; j < this.data[i].length; j++) {
                this.data[i][j] -= matrix.data[i][j];
            }
        }
        return this;
    }
    // multiply
    public MutableMatrix multiply(MutableMatrix matrix) {
        if (this.data[0].length != matrix.data.length) { // mxn * nxp
            throw new IllegalArgumentException("\33[31mInvalid dimensions for multiplication.\33[0m");
        }
        double[][] result = new double[this.data.length][matrix.data[0].length]; // mxp
        for (int i = 0; i < this.data.length; i++) { // rows of m
            for (int j = 0; j < matrix.data[0].length; j++) { // cols of p
                for (int k = 0; k < this.data[0].length; k++) { // cols of n
                    result[i][j] += this.data[i][k] * matrix.data[k][j];
                }
            }
        }
        this.data = result;
        return this;
    }

    // convert fromInmutable
    public static MutableMatrix fromInmutable(InmutableMatrix inmutableMatrix) {
        return new MutableMatrix(inmutableMatrix.getData());
    }

    public double[][] getData() {
        return data; // return reference to internal data, changes to this will change the matrix
    }
}


// Immutable matrix class
class InmutableMatrix {
    private final double[][] data;

    // Ctor
    public InmutableMatrix(double[][] data) {
        // deep copy of data
        this.data = new double[data.length][data[0].length];
        for (int i = 0; i < data.length; i++) {
            System.arraycopy(data[i], 0, this.data[i], 0, data[i].length);
        }
    }

    // add
    public InmutableMatrix add(InmutableMatrix matrix) {
        if (this.data.length != matrix.data.length || this.data[0].length != matrix.data[0].length) {
            throw new IllegalArgumentException("\33[31mMatrices must have the same dimensions.\33[0m");
        }
        double[][] result = new double[this.data.length][this.data[0].length];
        for (int i = 0; i < this.data.length; i++) {
            for (int j = 0; j < this.data[i].length; j++) {
                result[i][j] = this.data[i][j] + matrix.data[i][j];
            }
        }
        return new InmutableMatrix(result); // return new instance
    }
    // subtract
    public InmutableMatrix subtract(InmutableMatrix matrix) {
        if (this.data.length != matrix.data.length || this.data[0].length != matrix.data[0].length) {
            throw new IllegalArgumentException("\33[31mMatrices must have the same dimensions.\33[0m");
        }
        double[][] result = new double[this.data.length][this.data[0].length];
        for (int i = 0; i < this.data.length; i++) {
            for (int j = 0; j < this.data[i].length; j++) {
                result[i][j] = this.data[i][j] - matrix.data[i][j];
            }
        }
        return new InmutableMatrix(result);
    }
    // multiply
    public InmutableMatrix multiply(InmutableMatrix matrix) {
        if (this.data[0].length != matrix.data.length) {
            throw new IllegalArgumentException("\33[31mInvalid dimensions for multiplication.\33[0m");
        }
        double[][] result = new double[this.data.length][matrix.data[0].length];
        for (int i = 0; i < this.data.length; i++) {
            for (int j = 0; j < matrix.data[0].length; j++) {
                for (int k = 0; k < this.data[0].length; k++) {
                    result[i][j] += this.data[i][k] * matrix.data[k][j];
                }
            }
        }
        return new InmutableMatrix(result);
    }

    // convert fromMutable
    public static InmutableMatrix fromMutable(MutableMatrix mutableMatrix) {
        return new InmutableMatrix(mutableMatrix.getData());
    }

    public double[][] getData() {
        InmutableMatrix im = new InmutableMatrix(data); // return new instance with same data
        return im.data;
    }
}


// Test class
public class MatrixTest {

    public static void main(String[] args) {
        System.out.println("\33[44m====== Testing Matrices ======\33[0m\n");
        
        // test mutable matrix
        MutableMatrix m1 = new MutableMatrix(new double[][]{{1, 2}, {3, 4}});
        MutableMatrix m2 = new MutableMatrix(new double[][]{{5, 6}, {7, 8}});
        
        m1.add(m2).subtract(new MutableMatrix(new double[][]{{1, 1}, {1, 1}})); // linear operations
        System.out.println("\33[32mMutable Matrix(add, sub):\33[0m");
        printMatrix(m1.getData());

        m1.multiply(m2); 
        System.out.println("\33[32mMutable Matrix(mul):\33[0m");
        printMatrix(m1.getData());

        
        // test immutable matrix
        InmutableMatrix im1 = new InmutableMatrix(new double[][]{{1, 2}, {3, 4}});
        InmutableMatrix im2 = new InmutableMatrix(new double[][]{{5, 6}, {7, 8}});
        
        InmutableMatrix imResult = im1.add(im2).subtract(new InmutableMatrix(new double[][]{{1, 1}, {1, 1}}));
        System.out.println("\33[33mImmutable Matrix(add, sub):\33[0m");
        printMatrix(imResult.getData());

        InmutableMatrix imResult2 = im1.multiply(im2);
        System.out.println("\33[33mImmutable Matrix(mul):\33[0m");
        printMatrix(imResult2.getData());

        // test conversion
        MutableMatrix m3 = MutableMatrix.fromInmutable(im1);
        System.out.println("\33[34mMutable Matrix(fromInmutable):\33[0m");
        printMatrix(m3.getData());

        InmutableMatrix im3 = InmutableMatrix.fromMutable(m1);
        System.out.println("\33[35mInmutable Matrix(fromMutable):\33[0m");
        printMatrix(im3.getData());

        // test mutability / immutability
        m1.getData()[0][0] = 10; // try to change mutable matrix
        System.out.println("\33[36mMutable Matrix(changed):\33[0m");
        printMatrix(m1.getData());

        im1.getData()[0][0] = 10; // try to change immutable matrix
        System.out.println("\33[36mImmutable Matrix(changed):\33[0m");
        printMatrix(im1.getData());
    }

    private static void printMatrix(double[][] matrix) {
       for (double[] row : matrix) {
           for (double value : row) {
               System.out.print(value + " ");
           }
           System.out.println();
       }
   }
}
