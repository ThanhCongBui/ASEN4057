double BC(double x, double y, int probindex){
    double g;
    if (probindex == 1) {
        g = x;
    }
    else if (probindex == 5) {
        g = 70+273;
    }
    else{
        g = 0;
    }
    return g;
}
