[![header](../../../assets/header22.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# 深度学习正则化系列7：Dropout

《Deep Learning》Chapter 7
Regularization for Deep Learning
翻译水平有限，如有错误请留言，不胜感激！

[toc]
<h1>7.12 Dropout</h1>
Dropout（Srivastava等人，2014）是一种计算量不大但功能强大的方法，可用来正则一大类模型。第一种近似下，dropout可以被认为是一个方法，使许多大型神经网络能够集成的更实用的Bagging方法。Bagging涉及训练多个模型，并在每个测试样本上评估多个模型。当每个模型是大型神经网络时就不切实际了，因为训练和评估这样的网络在运行时间和所需内存方面的代价大。通常使用五到十个神经网络集成——Szegedy等人（2014a）使用六个神经网络赢得ILSVRC比赛，但超过这个数目会使集成的结果迅速变得笨拙。即使是对指数级数目的神经网络做集成，dropout也是一种低成本的近似训练和评估一个bagged的集成模型的方法。

<!--more-->

特别地，dropout训练由所有子网络组成的整体，如图7.6所示，这个整体中的子网络每个可以通过从原本的基础网络中去除某些非输出单元形成。在大多数现代神经网络中，基于一系列的变换和非线性操作，我们可以通过将其输出值乘以零来从网络中有效地移除该单元。该过程需要对模型进行一些轻微修改，如径向基函数网络中网络单元的状态和一些参考值。在这里为简单起见，我们提出了以零乘法为基础的dropout算法，但是它可以被简单地与网络中移除单元的其它操作一起工作。

回想一下，用bagging学习，我们定义k个不同的模型，通过从具有替换的训练集中抽样构建 $k$ 个不同的数据集，然后在数据集 $i$ 上训练模型 $i$ 。Dropout的目的是近似这个过程，而不是构造指数数量规模的神经网络。具体来说，为了借助dropout进行训练，我们使用一个基于小批量样本的学习算法，每次更新是小步骤的，好比随机梯度下降。每次将一个样本加载到一个小批量样本中，我们随机抽样一个不同的二进制掩码，以应用于网络中的所有输入和隐藏单元。每个单元的掩码独立于所有其它单元进行采样。采样掩码值为 $1$ 的概率（也导致一个单元会被包括进去）是在训练开始之前就被确定的超参数。它不是当前模型参数或输入样本的函数。通常以 $0.8$ 的概率包括输入单元，以 $0.5$ 概率包括隐含单元。然后我们照常运行正向、反向传播和学习更新。图7.7说明了如何使用dropout运行正向传播。

更正式地说，假设有掩码向量 $\mu$ 其每个元素为指定包含的单元，并且 $J(\theta, \mu)$ 定义了模型的损失，参数 $\theta$ 和掩码 $\mu$ 定义了模型。dropout训练包括最小化 $E_{\mu} J(\theta, \mu)$ 。期望包含指数个项，但是我们可以通过对掩码向量 $\mu$ 的采样值获得其梯度的无偏估计。

Dropout训练与bagging训练不完全相同。在bagging策略下，模型都是独立的。在dropout策略下，模型共享参数，每个模型继承来自父神经网络参数的不同子集。该参数共享使得用少量的内存就可以表示指数数量的模型。在bagging策略下，每个模型的训练都会收敛其相应的训练集。在dropout策略下，通常大多数模型没有被明确训练——通常当模型足够大时，若有足够多的时间，采样所有可能的子网络是可行的。相反，单步更新是基于采样到的子网络的训练，并且参数共享使得剩余子网络的参数达到较好值。这些是dropout和bagging的唯一区别。除此之外，dropout与bagging算法一致。例如，每个子网络的训练集合是用替换采样原始训练集合的方法得到的子集。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/01/deeplearning_chapter7_figure7.6.png" alt="" width="949" height="928" />

<strong>图7.6：dropout的训练基于所有子网络组成的集成，可以通过从底层基础网络中删除非输出单元后得到的网络来构建。这里，我们从具有两个可见单元和两个隐含单元的基本网络开始。这四个单元有十六中可能的网络子集。改图展示了从原始网络中丢弃不同的单元子集而形成的所有十六个子网络。在这个小例子中，所得到的网络的大部分没有输入单元或没有将输入连接到输出的路径。这个问题相比具有较宽层的网络变得不重要，丢弃从输入到输出的所有可能路径的对较宽层网络的概率更低。</strong>

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/01/deeplearning_chapter7_figure7.7.png" alt="" width="454" height="900" />

<strong>图7.7：使用dropout策略下网络正向传播的示例。（在上图中）有两个输入单元，一个隐藏层和两个隐藏单元以及一个输出单元的前馈网络。（在下图中）是dropout策略下的正向传播，对于网络中的每个输入或隐藏单元，随机抽取矢量 $\mu$ 中的一个元素。 <strong>$\mu$ </strong>的元素都是二进制的，并且彼此独立地采样。每个元素为 $1$ 的概率是超参数，对于隐藏层该超参数通常为 $0.5$ ，而输入层通常为 $0.8$ 。网络中的每个单元乘以相应的掩码，然后正常传播继续通过网络的其余部分。这相当于从图7.6中随机选择一个子网络，并向前传播。</strong>

为了做出预测，一个bagging策略的集成模型的结果必须依赖所有成员的累积投票。我们将这个过程称为推断。到目前为止，我们对bagging和dropout的描述不需要模型明确的概率。现在，我们假设模型的作用是输出概率分布。 在装袋的情况下，每个模型 $i$ 产生概率分布 $p^{(i)} (y|x)$。 整体的预测由所有这些分布的算术平均值给出，

$$
\frac{1}{k} \sum_{i=1}^{k} p^{(i)} (y | x).
$$

在dropout策略下，由掩码向量 $\mu$ 定义的每个子模型，每个自模型定义概率分布 $p(y | x, \mu)$ 。 所有掩码的算术平均值由下式给出

$$
\sum_{\mu} p(\mu) p(y|x,\mu)
$$

其中 $p(\mu)$ 是用于在训练时间采样 $\mu$ 的概率分布。

因为这个加和包括指数数目的项，除非模型结构允许某种形式简化的情况下，否则难以评估。到目前为止，深层神经网不允许任何易于理解的简化。相反，我们可以用抽样来近似推理，通过对许多掩码的输出求平均来近似。即使是10-20个掩码，通常也足以获得良好的性能。

然而有一个更好的方法，能够仅以一个前向传播的代价获得整个集成模型预测的良好近似。为此，在集成模型成员的预测分布上我们从几何平均数改用算术平均数。Warde-Farley等人（2014）提出的论点和实证证据表明在这种情况下几何平均值与算术平均值等价。

多个概率分布的几何平均值不能保证为概率分布。为了保证结果是一个概率分布，我们强加的要求是，没有一个子模型将概率 $0$ 分配给任何事件，并且我们对结果分布进行重新归一化。由几何平均直接定义的非规格化概率分布（unnormalized probability distribution）由下式给出

$$
\begin{align}
\tilde{p}_{\text{ensemble}}(y \mid x) = \sqrt[2^d]{\prod_{\mu} p(y \mid x, \mu)},
\end{align}
$$

其中 $d$ 是可以丢弃的单元数。这里我们使用均匀分布 $\mu$ 来简化表示，但非均匀分布也是可能的。为了做出预测，我们必须重新标准化集成模型（re-normalize the ensemble）：

$$
\begin{align}
p_{\text{ensemble}}(y \mid x) = \frac{\tilde{p}_{\text{ensemble}}(y \mid x)}
{\sum_{y'}\tilde{p}_{\text{ensemble}}(y' \mid x) }.
\end{align}
$$

涉及dropout的一个关键的见解（Hinton等人，2012c）是，我们可以在一个模型中评估 $p(y|x)$ 来近似 $p_{\text{ensemble}}$ ：具有所有单元的模型，但是单元 $i$ 的权重乘以包括单元 $i$ 的概率。此修改的动机是捕获该单元输出的正确期望值。我们将这种方法称为权重缩放推理规则（weight scaling inference rule）。在深度非线性网络中，这个近似推理规则的精度还没有任何理论论证，但在实验中它表现很好。

因为我们通常使用 $\frac{1}{2}$ 的包含概率，所以权重缩放规则通常等于在训练结束时将权重除以 $2$ ，之后的过程就和往常无异。实现相同结果的另一方式是在训练期间将单元的状态乘以 $2$ 。无论哪种方式，目标是确保在测试时单元的预期总输入与在训练时到该单元的预期总输入大致相同，即使训练中平均下来会有一半的单元会丢失。

对于没有非线性隐含单元的许多类型的模型而言，加权缩放推理规则（weight scaling inference rule）是精确的。对于一个简单的例子，考虑一个softmax回归分类器，输入变量是有着 $n$ 个元素的向量 $v$ 表示：

$$
\begin{align}
P(y = y \mid v) = \text{softmax}\big(W^\top v + b \big)_y.
\end{align}
$$

我们可以通过输入的二进制向量 $d$ 的逐元素乘法来索引到子模型族中：

$$
\begin{align}
P(y = y \mid v; d) = \text{softmax}\big(W^\top(d \odot v) + b \big)_y.
\end{align}
$$

集成结果的预测是根据集成的所有模型成员预测的几何平均值进行重新规范化来定义从而得到的预测值：

$$
\begin{align}
P_{\text{ensemble}}(y = y \mid v) = \frac{\tilde{P}_{\text{ensemble}}(y = y \mid v)}
{\sum_{y'}\tilde{P}_{\text{ensemble}}(y = y' \mid v) },
\end{align}
$$

其中有

$$
\begin{align}
\tilde{P}_{\text{ensemble}}(y=y \mid v) =
\sqrt[2^n]{\prod_{d \in \{0,1\}^n} P(y = y \mid v; d)}.
\end{align}
$$

为了看到权重缩放规则是精确的，我们可以简化 $\widetilde{P}_{ensemble}$ ：

$$
\begin{align}
\tilde{P}_{\text{ensemble}}(y=y \mid v) =
\sqrt[2^n]{\prod_{d \in \{0,1\}^n} P(y = y \mid v; d)} \\
= \sqrt[2^n]{\prod_{d \in \{0,1\}^n} \text{softmax}(W^\top(d \odot v) + b)_y} \\
= \sqrt[2^n]{\prod_{d \in \{0,1\}^n} \frac{\exp (W_{y,:}^\top(d \odot v) + b_y)}
{\sum_{y'}\exp (W_{y',;}^\top(d \odot v) + b_{y'})}}\\
= \frac{\sqrt[2^n]{\prod_{d \in \{0,1\}^n}\exp (W_{y,:}^\top(d \odot v) + b_y)}}
{ \sqrt[2^n] \prod_{d \in \{0,1\}^n} \sum_{y'}\exp (W_{y',:}^\top(d \odot v) + b_{y'})}
\end{align}
$$

因为 $\widetilde{P}$ 将被归一化，我们可以安全地忽略与因子的乘法操作，其中因子相对于 $y$ 是常数：

$$
\begin{align}
\tilde{P}_{\text{ensemble}}(y=y \mid v) &amp;\propto
\sqrt[2^n]{\prod_{d \in \{0,1\}^n} \exp (W_{y,:}^\top(d \odot v) + b_y)} \\
&amp; = \exp \Bigg(\frac{1}{2^n} \sum_{d \in \{0,1\}^n} W_{y,;}^\top(d \odot v) + b_y \Bigg) \\
&amp; = \exp \Big(\frac{1}{2}W_{y,:}^\top v + b_y \Big) .
\end{align}
$$

将其代入方程7.58（$\begin{align} P_{\text{ensemble}}(y = y \mid v) = \frac{\tilde{P}_{\text{ensemble}}(y = y \mid v)} {\sum_{y’}\tilde{P}_{\text{ensemble}}(y = y’ \mid v) }, \end{align}$），我们得到一个softmax分类器，权重为 $\frac{1}{2}W$ 。

权重缩放规则在其它场景的设定中也是精确的，包括具有条件正态输出的回归网络，以及无在隐含层没有非线性单元的深层网络。然而，权重缩放规则只是对具有非线性的深层模型的一种估计。虽然近似没有在理论上的足够证明，但在实际中却有效。Goodfellow等人（2013a）实验发现，对于集成模型，权重缩放近似可以比蒙特卡罗近似效果更好（在分类的精度方面）。即使当蒙特卡罗近似允许采样多达1,000个子网络时，这也成立。Gal和Ghahramani（2015）发现，一些模型使用二十个样本和蒙特卡罗近似可以获得更好的分类精度。这看来推理近似的最佳方法选择是和问题相关的。

Srivastava等人（2014）表明，dropout比其它代价低的标准正则化算法更有效，例如权重衰减（weight decay），过滤范数约束（filter norm constraints）和稀疏活动正则化（sparse activity regularization）。Dropout还可以与其它形式的正则化组合实现其进一步改进。

Dropout的一个优点是它在计算效率高。在训练期间使用dropout每次更新的每个样本仅需要 $O(n)$ 时间复杂度的计算，以生成 $n$ 个随机二进制数并将它们乘以各自神经元的状态。该过程实现后，它还可能需要 $O(n)$ 的空间复杂度来存储这些二进制数，直到反向传播阶段。尽管在开始对样本执行推断前必须将权重除以 $2$ ，但在训练模型时的推理具有的损失在每个样本上的计算都是相同的，就好像我们没有使用dropout策略。

Dropout的另一个重要优点是它不会很大程度地限制所使用模型的类型或训练算法。它几乎适用于任何以分布式表征样本、且可以使用随机梯度下降训练的模型。这包括前馈神经网络，概率模型如受限玻尔兹曼机（restricted Boltzmann machines，Srivastava等人，2014）和递归神经网络（recurrent neural networks，Bayer和Osendorfer，2014；Pascanu等人，2014a）。许多其它类似强有力的正则化策略对模型的架构施加了更严格的限制。

虽然将dropout应用到特定模型的每步损失值是可以忽略的，但在完整系统中使用dropout的损失值可能是明显的。因为dropout是一种正则化技术，它降低了模型对数据的有效容量。要抵消这种影响，我们必须增加模型的大小。通常当使用dropout时的最佳验证集误差要比不用时低得多，但是这是以大得多的模型和更多迭代步数的训练算法为代价的。对于非常大的数据集，正则化使得泛化误差几乎没有减少。在这些情况下，使用dropout的较大模型的计算成本的代价要超过常规正则化手段带来的好处。

当带标记的训练本数量很小时，dropout不太有效。贝叶斯神经网络（Neal，1996）在替代拼接数据集（Alternative Splicing Dataset，Xiong等人，2011）上的表现优于dropout策略，替代拼接数据集（Alternative Splicing Dataset）中的可用样本数少于5,000（Srivastava等人，2014）。当有额外的未标记数据可用时，无监督特征学习可以获得优于dropout的优点。

Wager等人（2013）表明，当应用于线性回归时，dropout相当于 $L^2$ 权重衰减的正则化法，每个输入特征具有不同的权重衰减系数。每个特征的正则化系数的大小由其方差确定。类似的结果适用于其它线性模型。对于深度模型，辍学不等于权重衰减的正则化法。

在使用dropout训练时所使用的随机性对于该方法的成功不是必需的。它只是近似所有子模型之和的一种手段。Wang和Manning（2013）衍生出了边缘化的分析近似。它们的近似由于在梯度计算中随机性会减少，被称为快速dropout，可以加快收敛时间。该方法还可在测试时使用，作为在所有子网络上做平均的、比起权重缩放近似更为原始的（但计算代价大）近似法。在小规模神经网络对问题的解决性能上，快速dropout已接近标准的性能表现，但尚未产生显著的性能提升或应用于大问题上。

正如随机性不是dropout实现正则化效果所必须的，dropout的正则能力也是不够的。为了证明这点，Warde-Farley等人（2014）在控制实验中设计了一种称为dropout boosting的方法，dropout boosting使用与dropout完全相同的掩码噪声，但缺乏正则的效果。dropout boosting训练整个集成模型联合地最大化训练集上的对数似然。在传统意义上，传统的dropout类似于bagging，这种方法类似于boosting。如预期的那样，与将整个网络作为单个模型进行训练相比，使用dropout boosting的实验几乎表现不出正则化效果。这表明，作为bagging的dropout的解释具有超过对噪声鲁棒性的dropout的解释的价值。bagged集成模型的正则化效果，仅在随机抽样集成模型中的每单个成员模型表现良好时才能实现。

Dropout已经启发了其它随机方法来训练具有指数量级数目的模型在集成时共享权重。DropConnect是一种特殊情况，其中单个标量权重和单个隐藏单元状态之间的每个乘积被认为是可以舍弃的单元（Wan等人，2013）。随机池化（stochastic pooling）是用于建立单个卷积网络集成到一起的随机池化形式（参见第9.3节），其中每个卷积网络给出每个特征图的不同空间位置。到目前为止，dropout仍然是最被广泛使用的隐式集成方法。

Dropout的一个关键想法是，训练具有随机行为的网络，并通过对多个随机决策取平均来做出预测，实现了一种具有参数共享的bagging形式。早些时候，我们描述了dropout作为对一个由可留或可丢弃单元形成的模型集合的bagging。然而，这个模型平均策略不需要基于单元的保留和舍弃。原则上，任何种类的随机修改都是允许的。在实践中，我们必须选择神经网络能够学习抵抗的一系列修正策略。理想情况下，我们还应该使用允许快速近似推理规则的模型集合。我们可以认为由向量 $\mu$ 参数化的任一修改形式，都作为训练所有 $\mu$可能取值的 $p(y | x, \mu)$ 的集成模型（We can think of any form of modiﬁcation parametrized by a vector $\mu$ as training an ensemble consisting of $p(y | x, \mu)$ for all possible values of $\mu$.）。没有要求 $\mu$ 具有一定数量的值。例如， $\mu$ 可以是实值的。Srivastava等人（2014）表明，将权重乘以 $\mu N(1,I)$ 可以优于基于二进制掩码的dropout策略。因为 $E[\mu] = 1$ ，所以标准网络自动在集成模型中实现近似推理，而不需要任何权重缩放。

到目前为止，我们已经描述了dropout完全作为一种高效，近似bagging的策略。然而，还有另一个更加深入的dropout观点。Dropout训练不仅是一个bagging的模型集成策略，而是共享隐含单元模型的模型集合策略。这意味着每个隐含单元必须能够很好地执行，而不管模型中存在哪些其它隐含单元。隐含单元必须准备在模型之间交换和互换。Hinton等人（2012c）的灵感来自生物学的一个想法：有性生殖，涉及在两个不同的生物之间交换基因，创造进化压力的基因，变得不但好且在不同生物间变得容易交换。这样的基因和此类特征对于其环境中的变化是非常鲁棒的，因为它们不能不正确地适应任何一种生物或模型的不寻常特征。因此，dropout正则化后的每个隐含单元，不仅是一个好特征，而且该特征在许多环境下都表现良好。Warde-Farley等人（2014）将dropout训练与大的集成模型的训练进行了比较，并得出结论，dropout在泛化误差的提升上远超独立模型集成得到的改进。

重要的是要理解，dropout起作用的原因大部分来自掩码噪声被施加到隐含单元上这一事实。这可以被看作是对输入信息内容的高度智能自适应破坏的形式，而不是破坏输入的原始值。例如，如果模型通过学习隐藏单元 $h_i$ 来确定鼻子从而检测人脸，那么当 $h_i$ 被丢掉就相当于图像中存在鼻子的信息被擦除了。模型必须学习另一个 $h_i$ ，或者对鼻子的存在进行冗余编码，或者通过另一个特征（例如嘴）来检测人脸。除非噪声很大以至于图像中几乎所有信息都被去除，否则在输入处添加非结构化噪声的传统噪声注入技术不能从面部图像随机擦除关于鼻子的信息。破坏提取的特征而不是原始特征，才能使得破坏过程可以利用模型到目前为止从输入数据的分布获取到的所有知识。

Dropout的另一个重要方面是噪声实现过程是乘法的。如果噪声与固定尺度相加，则具有附加噪声 $\epsilon$ 的线性校正单元（ReLU）的激活隐含单元 $h_i$ 可以轻易地学习这会使 $h_i$ 值变得非常大，以便通过比较使添加的噪声 $\epsilon$ 不明显（If the noise were additive with ﬁxed scale, then a rectiﬁed linear hidden unit hi with added noise $\epsilon$ could simply learn to have hi become very large in order to make the added noise $\epsilon$ insigniﬁcant by comparison.）。乘法噪声不允许这种针对噪声鲁棒性问题的异常解决方案。

另一种深度学习算法，批量归一化（batch normalization），是在训练时在隐含单元上引入加性和乘性噪声的方式对模型参数进行重新参数化的方法。批量归一化的主要目的是提升优化性能，噪声也有正则化效果，并且有时使dropout变得不必要。在第8.7.1节中详细描述了批量归一化（batch normalization）。
