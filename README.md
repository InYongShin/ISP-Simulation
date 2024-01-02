# Image Signal Processing (ISP)

태그: C++
기간: 2022/09/01 → 2022/12/31

### **프로젝트 개요**

◾ 개발인원 : 개인 프로젝트

◾ 고급렌더링특론1 과목 개인 프로젝트

◾ 카메라 Raw Data부터 사용자한테 보여지기까지의 과정인 ISP를 직접 구현하는 프로젝트

### 주요 내용

◾ **OpenCV를 사용하지 않고 개발**

◾ **Bayer Image**

▶ Sony camera와 Galaxy smartphone camera로 찍은 RAW Data를 사용

▶ 카메라 센서 기본값인 Black level을 제거한 후에 연산 시작

▶ (Denoising) Bayer에서 Denoising을 하는 딥러닝 기반 K-sigma 기술 적용

![Untitled](https://github.com/InYongShin/ISP-Simulation/assets/57998317/04bcfe07-db93-4ce4-87a9-050dd391af96)

![Untitled 1](https://github.com/InYongShin/ISP-Simulation/assets/57998317/71c26ef4-df2c-4e6d-aec6-dcd7c9cee643)

**◾ Demosaicing**

▶ 다른 Bayer 패턴을 가진 두 카메라에 대해 각각 Linear interpolation

![Untitled 2](https://github.com/InYongShin/ISP-Simulation/assets/57998317/cb02ce7e-bc4c-4fe6-b429-4c192ebdf995)

![Untitled 3](https://github.com/InYongShin/ISP-Simulation/assets/57998317/2368a40f-90b2-4477-b15e-fd7de7d2a443)

![Untitled 4](https://github.com/InYongShin/ISP-Simulation/assets/57998317/80006bc4-6ff0-4e39-80c2-31a6500af5da)

**◾ Denoising**

▶ Noise 제거를 위한 Gaussian, Median, Bilateral filter 구현 및 적용

![Untitled 5](https://github.com/InYongShin/ISP-Simulation/assets/57998317/4e2b8033-4bc4-4749-b8ca-d93e1e3d6117)

**◾ Color converting**

▶ Camera RGB(Unknown space) → sRGB(Known space) 로의 Color 변환

▶ Reference color checker로 Spyder checkr 사용

▶ Least Squares Problem

![Untitled 6](https://github.com/InYongShin/ISP-Simulation/assets/57998317/1d4ce01c-bf11-4f2d-af98-44f0b3523cd8)

**◾ Auto Exposure (Metering)**

▶ Center weighted average

▶ Spot

![Untitled 7](https://github.com/InYongShin/ISP-Simulation/assets/57998317/e87d882c-d158-4803-bb02-29e180fe99aa)

![Untitled 8](https://github.com/InYongShin/ISP-Simulation/assets/57998317/65d5f909-d09c-4809-b2a3-4fa63292a24a)

**◾ Gamma correction**

▶ 이미지 저장할 때(사용자에게 보여질 때) sRGB Gamma 적용

![Untitled 9](https://github.com/InYongShin/ISP-Simulation/assets/57998317/6a5cca7e-45b0-406f-9d7f-fab1dff9085f)

### 프로젝트 **도식화**

![Untitled 10](https://github.com/InYongShin/ISP-Simulation/assets/57998317/967365ce-9d6e-48be-b7d8-c5ef047f66fa)

### 결과

![Untitled 11](https://github.com/InYongShin/ISP-Simulation/assets/57998317/ca780a2a-7e59-480c-b079-f54b93c0b71f)

![Untitled 12](https://github.com/InYongShin/ISP-Simulation/assets/57998317/3ab2473b-2215-41b7-8af8-51baf58f8af2)

![Untitled 13](https://github.com/InYongShin/ISP-Simulation/assets/57998317/1de3ee64-ecf9-4b84-895c-0bcd9cbc6a42)

![Untitled 14](https://github.com/InYongShin/ISP-Simulation/assets/57998317/adb2734d-2ee1-43e8-b468-10f1630fd074)
