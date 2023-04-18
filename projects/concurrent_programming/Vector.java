public class Vector {
    private double x;
    private double y;

    private static volatile Vector currentSum;
    private static volatile double currentDot;

    public void addX(double x) {
        this.x += x;
    }

    public void addY(double y) {
        this.y += y;
    }
    public Vector(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double getX() {
        return this.x;
    }

    public double getY() {
        return this.y;
    }

    private static class getSum implements Runnable {
        private Vector other;

        public getSum(Vector other) {
            this.other = other;
        }

        @Override
        public void run() {
            currentSum.addX(other.getX());
            currentSum.addY(other.getY());
        }
    }

    private static class getDot implements Runnable {
        private Vector other;
        private Vector vec;


        public getDot(Vector vec, Vector other) {
            this.vec = vec;
            this.other = other;
        }

        @Override
        public void run() {
            currentDot += (vec.getX() * other.getX() + vec.getY() * other.getY());
        }
    }

    public Vector sum (Vector other) {
        double length  = Math.sqrt(other.getX() * other.getX() + other.getY() * other.getY());
        double s = 10 / length;
        Vector v = new Vector(other.getX() * s, other.getY() * s);
        currentSum = new Vector(this.x, this.y);
        double rX = other.getX();
        double rY = other.getY();
        
        int T = (int) length / 10 + 1;
        private Thread threads[] = new Thread[ROWS];
        
        int i = 0;
        for (; i < (int)length / 10; i++) {
            threads[i] = new Thread(new getSum(v));
            threads[i].start();
            rX -= v.getX();
            rY -= v.getY();
        }
        if (rX > 0) {
            threads[i] = new Thread(new getSum(new Vector(rX, rY)));
            threads[i].start();
            i++;
        }
        
        for (int j = 0; j < i; ++j) {
            try {
                threads[j].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        return currentSum;
    }

    public double dot (Vector other) {
        double length  = Math.sqrt(other.getX() * other.getX() + other.getY() * other.getY());
        double s = 10 / length;
        Vector v = new Vector(other.getX() * s, other.getY() * s);
        currentDot = 0;
        double rX = other.getX();
        double rY = other.getY();

        for (int i = 0; i < (int)length / 10; i++) {
            Thread t = new Thread(new getDot(this, v));
            t.start();
            try {
                t.join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            rX -= v.getX();
            rY -= v.getY();
        }
        if (rX > 0) {
            Thread t2 = new Thread(new getDot(this, new Vector(rX, rY)));
            t2.start();
            try {
                t2.join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        return currentDot;
    }

    public Vector sumSeq (Vector other) {
        double length  = Math.sqrt(other.getX() * other.getX() + other.getY() * other.getY());
        double s = 10 / length;
        Vector v = new Vector(other.getX() * s, other.getY() * s);
        double rX = other.getX();
        double rY = other.getY();

        Vector res = this;

        for (int i = 0; i < (int)length / 10; i++) {
            res.addX(v.getX());
            res.addY(v.getY());
            rX -= v.getX();
            rY -= v.getY();
        }
        if (rX > 0) {
            res.addX(rX);
            res.addY(rY);
        }

        return res;
    }

    public double dotSeq (Vector other) {
        double length  = Math.sqrt(other.getX() * other.getX() + other.getY() * other.getY());
        double s = 10 / length;
        Vector v = new Vector(other.getX() * s, other.getY() * s);
        double rX = other.getX();
        double rY = other.getY();

        double res  = 0;

        for (int i = 0; i < (int)length / 10; i++) {
            res += (this.x * v.getX() + this.y * v.getY());
            rX -= v.getX();
            rY -= v.getY();
        }
        res += (this.x * rX + this.y * rY);

        return res;
    }

    public void main() {
        int range = 100;
        double a  = Math.random() * range;
        double b  = Math.random() * range;
        double c  = Math.random() * range;
        double d  = Math.random() * range;

        Vector vec1 = new Vector(a, b);
        Vector vec2 = new Vector(c, d);
        Vector res = vec1.sum(vec2);
        System.out.println("Vector1: [" + (int)vec1.getX() + ',' + (int)vec1.getY() + ']' );
        System.out.println("Vector2: [" + (int)vec2.getX() + ',' + (int)vec2.getY() + ']' );
        System.out.println("Suma (wielowątkowo): [" + (int)res.getX() + ',' + (int)res.getY() + ']' );
        res = vec1.sumSeq(vec2);
        System.out.println("Suma (sekwencyjnie): [" + (int)res.getX() + ',' + (int)res.getY() + ']' );

        double dot = vec1.dot(vec2);
        System.out.println("Iloczyn skalarny (wielowątkowo): " + (int)dot);
        dot = vec1.dotSeq(vec2);
        System.out.println("Iloczyn skalarny (sekwencyjnie): " + (int)dot);
    }
}
