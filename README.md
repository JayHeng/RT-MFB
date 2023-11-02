# imxrt-multi-flash-boot-solution
一种灵活的i.MXRT下多串行NOR Flash型号选择的量产方案

![](/implementation_note.PNG)

### 已测试型号表

测试结果基于 RT1180-FLEXSPI-MEM-BB&DC_Rev.B（默认板卡设置）FlexSPI1 PortA MUX1

- 测试仅改动 BB-J68&J16 以设置 Flash_VCC: 3.3V/1.8V/1.2V
- 测试仅改动 BB-J11 以切换 QUAD/OCTAL

<table><tbody>
    <tr>
        <th>厂商</th>
        <th>型号</th>
        <th>电压</th>
        <th>类型</th>
        <th>备注</th>
    </tr>
    <tr>
        <td rowspan="8">华邦 Winbond</td>
        <td>W25Q40CLNIG</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>W25Q32JVSIQ</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q64NESIG</td>
        <td>1.14V - 1.26V</td>
        <td>QuadSPI 84MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q64JWSJQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q128JWSQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W25Q256JWBAQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>W25M512JWBIQ</td>
        <td>1.7 - 1.95V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>W35T51NW</td>
        <td>1.7 - 2.0V</td>
        <td>OctalSPI 100MHz DDR<br>
            OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td rowspan="12">旺宏 Macronix</td>
        <td>MX25V1635F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25V4035F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25V8035F</td>
        <td>2.3 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U6435F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U6432F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>JEDEC ID获取失败</td>
    </tr>
    <tr>
        <td>MX25L6433F</td>
        <td>2.65 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25U12832F</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td></td>
    </tr>
    <tr>
        <td>MX25L25645G</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>QE设置失败</td>
    </tr>
    <tr>
        <td>MX25U25645G</td>
        <td>1.65 - 2V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UW25345G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25UM51345G</td>
        <td>1.65 - 2V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>MX25LM51245G</td>
        <td>2.7 - 3.6V</td>
        <td>OctalSPI 200MHz DDR</td>
        <td></td>
    </tr>
    <tr>
        <td rowspan="8">兆易创新 GigaDevice</td>
        <td>GD32Q64CSIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 120MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD32LB64ESIG</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>回读校验失败</td>
    </tr>
    <tr>
        <td>GD32Q127CSIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 104MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD32B128ESIG</td>
        <td>2.7 - 3.6V</td>
        <td>QuadSPI 133MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD32LE128DSIG</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 120MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD32LT256EB1R</td>
        <td>1.65 - 2.0V</td>
        <td>QuadSPI 166MHz SDR</td>
        <td>通过</td>
    </tr>
    <tr>
        <td>GD32X512MEB2R</td>
        <td>2.7 - 3.6V</td>
        <td>OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
    <tr>
        <td>GD32LX512MEB1R</td>
        <td>1.65 - 2.0V</td>
        <td>OctalSPI 166MHz DDR<br>
            OctalSPI 200MHz DDR</td>
        <td>通过<br>
            回读校验失败</td>
    </tr>
</table>
