template<typename type> 
    class ARRAY{
public:
    type * tArray;
    int n;

public:
    ARRAY(){
        tArray = nullptr;
        n = 0;
    }

    ARRAY(const ARRAY& array){
        this->n = array.n;
        this->tArray = new type[n];
        
        for (int i = 0; i<n; i++){
            this->tArray[i] = array.tArray[i];
        }
    }

    ~ARRAY(){
        delete[] tArray;
        tArray = nullptr;
        n = 0;
    }
    ARRAY& operator=(ARRAY &array){
        delete[] this->tArray;

        this->n = array.n;
        this->tArray = new type[n];
        
        for (int i = 0; i<n; i++){
            this->tArray[i] = array.tArray[i];
        }

        return array;
    }

    void create(int n){
        delete[] this->tArray;
        this->tArray = new type[n];
    }
};
