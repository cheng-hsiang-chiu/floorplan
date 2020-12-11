// ...
//
// 
// class Floorplan {
//
//   friend class FloorplanTest;
//   friend class YourCompnayTester;
//  
//   public:
//     Floorplan();
//     void run();
//     void dump(std::ostream& os) const;
//   
//   private:
//
//     bool _is_expression_valid(const std::vector<int>& exp) const
// }
//
// 
// class FloorplanTester {
//
//    public:
//
//    bool is_expression_valid(const std::vector<int>& exp) const {
//      return _floorplanner._is_expression_valid(exp);
//    }
//
//    private:
//
//       Floorplan _floorplanner;
//
// }
//
