### Thông Tin Cá Nhân ###
#### Họ tên: ####
 Nguyễn Đăng Quang
#### MSSV: ####
 1512432 
#### Mail: ####
 nguyendangquangkt@gmail.com

### Các Chức năng Đã Làm Được ###
#### Các yêu cầu cơ bản ####
##### Thêm vào danh sách một mục chi, gồm 3 thành phần #####
- Loại chi tiêu, nội dung chi, số tiền.
 ( Ví dụ: Ăn uống, Ăn trưa, 15000. )
Loại chi tiêu được tạo mặc định gồm 6 loại chi tiêu:       
        + Ăn uống (Sáng, trưa, chiều, xế, tối, khuya, trà sữa…)
        + Di chuyển (bus, đổ xăng, taxi, bơm xe, tàu…), 
        + Nhà cửa (tiền thuê nhà, nước, điện, nước lau nhà…), 
        + Xe cộ (đổ xăng, vá xe, bảo dưỡng, gởi xe…), 
        + Nhu yếu phẩm (xà bông, sữa tắm, dao cạo râu…), 
        + Dịch vụ (intenet, thẻ cào điện thoại…).
##### Xem lại danh sách các mục chi của mình, lưu và nạp vào tập tin text #####
- Khi chương trình chạy lên tự động nạp danh sách chi tiêu từ tập tin systemadmin.txt lên và hiển thị (dùng listview ở chế độ fullrowselect )
- Khi chương trình thoát thì tự động lưu danh sách mới vào tập tin systemadmin.txt 
- Lưu dữ liệu dưới dạng tiếng việt có dấu (mã unicode)

##### Vẽ biểu đồ  nhằm biểu diễn trực quan tỉ lệ tiêu xài  #####
- vẽ biểu đồ tròn (pie chart) thể hiện tỉ lệ tiêu xài từng loại chi tiêu trên tổng số chi phí.
- Các chú thích phía dưới tương ứng với từng màu hiển thị tên loại chi tiêu ứng với từng màu trên biểu đồ, số tiền và phần trăm chiếm dụng trong tổng chi phí

#### các yêu cầu nâng cao đã làm ####
- Tập tin lưu trữ các khoản chi tiêu hỗ trợ lưu và nạp Tiếng Việt có dấu
- Vẽ biểu đồ hình tròn (pie chart) để biểu thị tỉ lệ chi tiêu.
- Cải tiến về mặt trải nghiệm người dùng (UX – User experience) và các chức năng bổ sung: 
    + Thêm chức năng "Add more " trong loại chi tiêu để người dùng có thể thêm các loại chi tiêu mà mình muốn ( max 10 loại)
    + New để làm mới lại sổ chi tiêu (có hỏi save trước khi làm mới)
    + Save as để lưu một sổ chi tiêu dưới dạng file text để xem lại khi cần
    + Open để mở một file thông tin chi tiêu đã lưu trước đó
    + click chuột phải sẽ có popup menu new,save, exit

### Các Luồng Sự kiện chính ( Main Flow) ### 
- Mở chương trình lên sẽ thấy màn hình chính để thêm loại chi tiêu, listview thể hiện các loại chi tiêu và thống kê các loại chi tiêu
- Bên phải màn hình chính là khu vực vẽ biểu đồ thể hiện tỉ lệ tiêu xài và chú thích cho từng loại chi tiêu
- khi nhập nội dung đầy đủ và bấm thêm thì sẽ tự động load nội dung đã thêm vào listview và vẽ lại biểu đồ thể hiện tỉ lệ tiêu xài
### Các Luồng Sự Kiện Phụ (add flow) ###
- Nhập thiếu nội dung chi tiêu sẽ báo lỗi chưa nhập dữ liệu
- nhập thiếu số tiền chi tiêu sẽ báo lỗi chưa nhập dữ liệu
- nhập sai định dạng số tiền chi tiêu sẽ thông báo lỗi 

### Link Youtube ###
https://goo.gl/Y7KJX8

### Hướng dẫn sử dụng ###
- Khi thêm full loại chi tiêu ( 10) muốn khởi tạo lại mặc định người dùng chỉ cần làm mới (New, nhớ save lại file để sau này xem lại) lại sổ chi tiêu và khởi động lại chương trình thì combo box loại chi tiêu sẽ quay lại là 6 loại mặc định
### Link tới RePo trên Bitbucket ###
https://bitbucket.org/QuangDangFIT/money-management-program

