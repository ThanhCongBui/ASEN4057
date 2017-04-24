double Conductivity(double x, double y, int probindex){
    double k;
    if (probindex == 4){
        if (x > 0.5){k = 20;}
        else{k = 1;}
    }
    else if(probindex == 5){
        if (0.01<x<0.015 && 0.01<y<0.015){k = 167;}
        else{k = 157;}
    }
    else{
        k = 1;
    }
    return k;
}
