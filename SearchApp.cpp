#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

/**
 * @brief Định nghĩa cha chung cho tất cả các thuật toán tìm kiếm
 * @author Lê Minh Triết - 20220045
 * @date 22/11/2024
 * @version 1.0
 */
class SearchAlgorithm {
    //Các biến để lấy thời gian tổng, sắp xếp, tìm kiếm
    protected: long long searchTime = 0;
    protected: long long buildTime = 0;
    protected: long long totalTime = 0;
    private: stack<system_clock::time_point> startTimes;
    private: system_clock::time_point endTime;
    //Lưu giữ vector cần tìm kiếm
    protected: vector<int> arr;

    public: vector<int> getArr() {
        return this->arr;
    }

    public: virtual void setArr(vector<int> arr) {
        this->arr = arr;
    }

    public: double getSearchTime() {
        return searchTime*1.0/1000;
    }

    public: double getBuildTime() {
        return buildTime*1.0/1000;
    }

    public: double getTotalTime() {
        return totalTime*1.0/1000;
    }

    //Bắt đầu một bộ đếm và thêm vào stack
    protected: void startTimeCount() {
        startTimes.push(high_resolution_clock::now());
    }
    //Lấy ra khỏi stack bộ đếm gần nhất và tính duration
    protected: long long getDuration() {
        endTime = high_resolution_clock::now();
        if(!startTimes.empty()) {
            system_clock::time_point startTime = startTimes.top();
            startTimes.pop();
            return duration_cast<microseconds>(endTime - startTime).count();
        }
        return 0;
    }

    protected: void setSearchAndTotalTime() {
        searchTime = getDuration();
        totalTime = getDuration();
    }

    /**
     * @brief In ra thông tin sau khi thực hiện thuật toán tìm kiếm,
     * bao gồm tên, các thời gian chạy.
     */
    public: void printInfo() {
        cout << "Run Time:: Total Time " << fixed << setprecision(3) << this->getTotalTime() << " ms; ";
        cout << "Build Time " << fixed << setprecision(3) << this->getBuildTime() << " ms; ";
        cout << "Search Time " << fixed << setprecision(3) << this->getSearchTime() << " ms.";
    }

    /**
     * @brief Trả về tên của thuật toán tìm kiếm
     * @return Một chuỗi là tên của thuật toán tìm kiếm
     */
    public: virtual string getAlgorithm() = 0;

    /**
     * @brief Thực hiện thuật toán tìm kiếm
     * @param value Giá trị cần tìm kiếm
     * @return Trả về vị trí xuất hiện đầu tiên của value,
     * hoặc -1 nếu không tìm thấy
     */
    public: virtual int search(int value) = 0;

    virtual~SearchAlgorithm() {};
};

class NeedSortedSearchAlgorithm: public SearchAlgorithm {
    //Biến đánh dấu dãy đã được sắp xếp chưa
    protected: bool isSorted = false;
    //Vector biểu diễn mảng sau khi sắp xếp và chỉ số cũ trong mảng
    protected: vector<pair<int, int>> sortedIndexArr;

    /**
     * @brief Kiểm tra xem một dãy có phải là dãy được sắp xếp tăng
     * @param &arr Tham chiếu tới vector cần kiểm tra
     * @return true nếu 
     */
    public: template<typename T> bool isAscending(const vector<T> &arr) {
        //Kiểm tra tăng
        for (int i = 0; i < (int) arr.size() - 1; i++) {
            if (arr[i] > arr[i+1]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Xây dựng một dãy được sắp xếp và lưu giữ vị trí các phần tử trong dãy cũ.
     */
    protected: void buildSortedArr() {
        int size = (int) arr.size();
        for (int i = 0; i < size; i++) {
            sortedIndexArr.push_back({arr[i], i});
        }
        if (isAscending(sortedIndexArr)) {
            isSorted = true;
            return;
        } 
        sort(sortedIndexArr.begin(), sortedIndexArr.end());
        isSorted = true;
    }

    public: void setArr(vector<int> arr) override {
        isSorted = false;
        sortedIndexArr.clear();
        this->arr = arr;
    }
};

class LinearSearch: public SearchAlgorithm {
    public: int search(int value) override {
        startTimeCount();
        startTimeCount();
        for (int i = 0; i < (int) arr.size(); i++) {
            if (arr[i] == value) {
                setSearchAndTotalTime();
                return i;
            }
        }
        setSearchAndTotalTime();
        return -1;
    }

    public: string getAlgorithm() override {
        return "LinearSearch";
    }
};

class BinarySearch: public NeedSortedSearchAlgorithm {
    public: int search(int value) override {
        startTimeCount();
        startTimeCount();
        if(!isSorted) {
            buildSortedArr();
        }
        buildTime = getDuration();
        startTimeCount();
        int left = 0, right = (int) sortedIndexArr.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (sortedIndexArr[mid].first == value) {
                setSearchAndTotalTime();
                return sortedIndexArr[mid].second;
            } else if (sortedIndexArr[mid].first < value) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        setSearchAndTotalTime();
        return -1;
    }

    public: string getAlgorithm() override {
        return "BinarySearch";
    }
};

class TernarySearch: public NeedSortedSearchAlgorithm {
    public: int search(int value) override {
        startTimeCount();
        startTimeCount();
        if(!isSorted) {
            buildSortedArr();
        }
        buildTime = getDuration();
        startTimeCount();
        int left = 0, right = (int) sortedIndexArr.size() - 1;
        while (left <= right) {
            int mid1 = left + (right - left) / 3;
            int mid2 = right - (right - left) / 3;
            if (sortedIndexArr[mid1].first == value) {
                setSearchAndTotalTime();
                return sortedIndexArr[mid1].second;
            } 
            if (sortedIndexArr[mid2].first == value) {
                setSearchAndTotalTime();
                return sortedIndexArr[mid2].second;
            } 
            if (value < sortedIndexArr[mid1].first) {
                right = mid1 - 1;
            } else if (value > sortedIndexArr[mid2].first) {
                left = mid2 + 1;
            } else {
                left = mid1 + 1;
                right = mid2 - 1;
            }
        }
        setSearchAndTotalTime();
        return -1;
    }

    public: string getAlgorithm() override {
        return "TernarySearch";
    }
};

class JumpSearch: public NeedSortedSearchAlgorithm {
    public: int search(int value) override {
        startTimeCount();       
        int step = (int) floor(sqrt((int) arr.size()));
        int prev = 0, size = (int) arr.size();
        startTimeCount();
        if(!isSorted) {
            buildSortedArr();
        }
        buildTime = getDuration();
        startTimeCount();
        while (sortedIndexArr[prev].first < value) {
            prev += step;
            if (prev >= size) {
                break;
            }        
        }
        for (int i = max(0, prev - step); i <= min(size, prev); i++) {
            if (sortedIndexArr[i].first == value) {
                setSearchAndTotalTime();
                return sortedIndexArr[i].second;
            }
        }
        setSearchAndTotalTime();
        return -1;
    }
    
    public: string getAlgorithm() override {
        return "JumpSearch";
    }
};

class SearchContext {
    private: SearchAlgorithm *searchAlgorithm;
    public: void setSearchAlgorithm(SearchAlgorithm *searchAlgorithm) {
        this -> searchAlgorithm = searchAlgorithm;
    }

    public: void setArr(vector<int> arr) {
        this->searchAlgorithm->setArr(arr);
    }

    public: int search(int value) {
        int result = this -> searchAlgorithm -> search(value);
        this->searchAlgorithm->printInfo();
        cout<<"Find "<<value<<" at "<<result<<endl;
        return result;
    }
};

class Tester {
    private: vector<pair<vector<int>, vector<int>>> tests;
    private: SearchContext searchContext;
    public: void addTest(const vector<int> &test, const vector<int> &values) {
        this->tests.push_back({test, values});
    }
    public: void testWith(const vector<int> &test, const vector<int> &value, SearchAlgorithm *searchAlgorithm) {
        cout << searchAlgorithm->getAlgorithm() << ": " << endl;
        searchAlgorithm->setArr(test);
        searchContext.setSearchAlgorithm(searchAlgorithm);
        for (int j = 0; j < (int) value.size(); j++) {
            searchContext.search(value[j]);
        }
    }

    public: void test() {
        LinearSearch linearSearch;
        BinarySearch binarySearch;
        TernarySearch ternarySearch;
        JumpSearch jumpSearch;
        
        for(int i = 0; i < (int) tests.size(); i++) {
            vector<int> valueI = tests[i].second;
            vector<int> testI = tests[i].first;
            cout<<"Test "<<i+1<<" -------:"<<endl;
            testWith(testI, valueI, &linearSearch);
            testWith(testI, valueI, &binarySearch);
            testWith(testI, valueI, &ternarySearch);
            testWith(testI, valueI, &jumpSearch);
        }
    }
};

int main() {
    vector<int> test1, test2;
    int N = 1e6;
    for(int i = 0; i < N; i++) {
        test1.push_back(i);
        test2.push_back(N+1-i);
    }
    vector<int> values;
    for(int i = 1; i <= 12; i++) {
        values.push_back(test1[i * N/13]);
    }
    Tester tester;
    tester.addTest(test1, values);
    tester.addTest(test2, values);

    tester.test();

    return 0;
}