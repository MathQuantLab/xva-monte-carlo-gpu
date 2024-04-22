# XVA Computation using Nested Monte Carlo and GPU Optimization

## Project Overview
This project implements the principles and strategies discussed in the article "XVA Principles, Nested Monte Carlo Strategies, and GPU Optimizations" for efficient X-valuation adjustments (XVA) computation. It utilizes nested Monte Carlo (NMC) methods optimized for graphics processing units (GPUs) to enhance the performance and accuracy of financial models dealing with Credit Valuation Adjustment (CVA), Funding Valuation Adjustment (FVA), and other related metrics.

## Background
Since the 2008 financial crisis, the need for accurate pricing and risk measurement tools that account for counterparty risks in derivative transactions has become crucial. This project builds on the groundwork laid by the article, which proposes an optimized approach to compute these adjustments using a nested Monte Carlo strategy on GPUs. This approach not only improves computational efficiency but also enhances the accuracy of simulations under complex market conditions.

## Features
- **Nested Monte Carlo Simulations**: Implements the nested simulation approach to handle multiple layers of dependence in XVA calculations, reducing computational complexity and variance in estimations.
- **GPU Optimization**: Utilizes GPU capabilities to parallelize computations, significantly speeding up the execution of Monte Carlo simulations.
- **Modular Design**: The codebase is structured to be modular, allowing easy adaptation and extension to various types of financial instruments and risk factors.

## Getting Started

### Prerequisites
- A CUDA-compatible GPU.
- Python 3.8 or highe.
- CUDA Toolkit (version recommended by your GPU manufacturer).

### Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/xva-nmc-gpu.git
   ```
2. Install required Python libraries:
   ```bash
   pip install -r requirements.txt
   ```

### Running the Tutorial
To explore how the XVA computations are performed using our implementation, you can run the Jupyter Notebook included:
```bash
jupyter notebook Tutorial.ipynb
```
This tutorial will guide you through the setup, execution, and interpretation of results step-by-step.

## Documentation
For more detailed information on the implementation and the methodology, refer to the `docs/` directory.

## Testing
Test the application to ensure reliability:
```bash
make test
```

## Contributing
Contributions to this project are welcome. See `CONTRIBUTING.md` for ways to get involved.

## License
This project is released under the GNU GENERAL PUBLIC LICENSE. See the `LICENSE.md` file for more details.

## Acknowledgments
- Original authors of the article for providing the theoretical framework and mathematical models used in this project.