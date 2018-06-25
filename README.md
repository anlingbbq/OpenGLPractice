# OpenGLPractice
学习opengl有一段时间了，简单展示下这个项目中 运用opengl做的有趣的事。
<br>学习自https://learnopengl.com/ 项目参考了OpenglSuperBible中的简易框架，做了简单的封装。</br>

#### 008.PhongLightingMode
* PointLights
<br>基础的Phong光照模型，图片展示点光源的效果，Phong光照模型由Ambient,Diffuse,Specular组成</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/008.PhongLightingMode.gif)

#### 010.LightCasters
* DirectionLights
<br>直射光，这里添加了纹理和高光贴图</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/010.LightCasters-DirectionLights.png)

* PointLights
<br>点光源，和之前相比添加了光照范围</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/010.LightCasters-PointLights.gif)

* SpotLights
<br>聚光灯，和一般的点光源相比，通过点乘计算夹角范围，判断是否计算difuse和specular</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/010.LightCasters-SpotLights.png)

### 011.MultipleLights
* MultipleLights
<br>分别计算各个光照，然后将颜色值的结果相加，在shader中多个点光源数量需要预先定义一个常亮</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/011.MultipleLights.png)

### 012.Model
* Model
<br>使用了模型加载库Assimp，Assimp会将不同格式的模型加载到自身的数据结构中，我们只需要处理这样的通用结构，而不用关心不同的文件格式。一个model包含多个mesh，mesh通常包含一组顶点，法线，纹理坐标等等，但只有一个vao，它是OpenGL中进行绘制的最小单位。</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/012.Model.png)

### 014.StencilTest
* StencilTest
<br>模板测试，模板缓冲可以基于场景中已经绘制的fragment，来决定是否丢弃特定的fragment。这一部分有些地方没搞懂，下图为描边的效果，实际上是绘制了两次cube，第一次绘制带纹理的cube，在对应的fragment的模板缓冲中设置为1，第二次绘制一个稍大些的绿色cube，并且只有模板缓冲中不等于1的时候才通过测试，从而只绘制边缘的效果。</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/014.StencilTest.png)

### 015.BlendingTest
* BlendingTest
<br>混合测试，通常用于透明效果的处理，在下图中，由于深度测试不关心混合结果，即使透明的部分应该显示背后的窗户，深度测试仍然会丢弃它们，对于当前场景，可以通过由远到近排序渲染的方式解决。</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/015.BlendingTest.png)

### 017.Framebuffers
* Framebuffers
<br>颜色，深度，模板等缓冲结合起来叫做帧缓冲，一般的绘制是在默认的帧缓冲上，如果我们渲染到一个不同的帧缓冲中被称作离屏渲染，通过离屏渲染附加纹理，我们可以对整体做一些非常有趣的后期效果！！</br>
* 正常效果
<br>![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/017.Framebuffers.png)</br>
* 颜色反转 像是开大招的时刻
<br>![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/017.Framebuffers-Inversion.png)</br>
* 模糊效果
<br>![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/017.Framebuffers-Blur.png)</br>

### 018.Cubemaps
* Reflection
<br>天空盒是非常实用的东西，但还有更有趣的，我们通过法线和视口方向计算反射方向，再对天空盒纹理采样，作为cube的颜色，得到一个非常漂亮的镜面立方体</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/018.Cubemaps-Reflection.png)
* Refraction
<br>同样可以实现折射效果，这里设置折射率模拟玻璃效果，并运用到模型上，除此之外，如果不使用天空盒，我们使用帧缓冲绘制一个sampleCube可以实现动态环境贴图的效果，但这样的开销非常大。</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/018.Cubemaps-Refraction.png)

### 020.GeometryShader
* GeometryShader
<br>几何着色器处于顶点和片段之间，它可以对所有的顶点做一些修改，下图中我们只传了4个顶点坐标，在gs中额外生成了两个顶点，原本绘制4个点的图元变成了绘制4条线</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/020.GeometryShader.png)
* ExplodingEffect
<br>我们通过取得每3个顶点，并沿着法线移动，可以循环移动每个三角形面，类似爆炸效果</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/020.GeometryShader-ExplodingObject.gif)
* DecomposeEffect
<br>在上面的基础上，稍作修改，实现更为酷炫的分解效果</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/020.GeometryShader-DecomposeObject.gif)

### 021.Instancing
* Instancing
<br>我们可以通过告诉OpenGL使用一个绘制函数，将一些数据绘制为多个物体，来极大的降低cup和gpu之间的通信，减少drawcall。这里的一些数据保存在vbo中，下图中我们绘制了10w个小陨石</br>
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/021.Instancing-AsteroidField1.png)
![](https://github.com/anlingbbq/OpenGLPractice/blob/master/Demonstrate/021.Instancing-AsteroidField2.png)


