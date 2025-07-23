# 📡 ESP32Website V1.1.2

**ESP32Website** là một thư viện hỗ trợ xây dựng giao diện web cho ESP32, bao gồm các chức năng xem log, gửi lệnh thời gian thực qua WebSocket, và truy cập/đọc dữ liệu từ thẻ SD. Dễ dàng tích hợp và mở rộng cho các dự án IOT, debug, giám sát thiết bị...

## 🔧 Tool chuyển định dạng html sang gzip
- Làm giảm dung lượng giao diện web nạp vào thiết bị
- Chạy file [convert.exe](https://github.com/duylonghn/ESP32Website/blob/main/WebUI/)
- Khi phần mềm được chạy hãy nhập chế độ gồm có `to_array` để chuyển từ `.html` sang `.h` và `to_html` là ngược lại
- Nhập tên file muốn chuyển
- Nhập tên file muốn đặt sau khi chuyển

## 📁 Thư viện con

### 1. `WebSerialMonitor 1.0.1`
> Giao tiếp WebSocket 2 chiều như Serial Monitor.

- Gửi lệnh từ trình duyệt tới ESP32.
- ESP32 phản hồi tức thời về client thông qua WebSocket.
- Hỗ trợ lệnh khởi động lại, bật/tắt thiết bị,...
- Theo dõi log Serial Monitor không cần kết nối cáp dữ liệu/

### 2. `WebReadSDCard 1.0.2`
> Thao tác với thẻ SD Card
- Xem thông tin thẻ nhớ: Dung lượng tổng, dung lượng đã sử dụng, dung lượng còn lại/
- Thao tác: Xóa, Thêm, Đổi tên file trên thẻ nhớ/
- Hàm dừng server

### 3. `ElegentOTA`
> Nạp firmware qua OTA
- Tạo 1 webserver cho phép người dùng tải và nạp firmware qua OTA mà không cần kết nối trực tiếp dành cho các thiết bị ở vị trí khó tiếp cận/
- Sử dụng thư viện có sẵn của ElegentOTA và được sửa lại giao diện web cho đồng bộ màu với thư viện.

## Cách sử dụng
### B1: Tải thư viện ESP32Website về máy
### B2: Tạo file .ino ngay trong folder ESP32Website
### B3: Cài đặt Sketchook location về Folder ESP32Website
![Preferences](Image/Preferences.png)
![Sketchhook location](Image/Sketchhook%20Location.png)