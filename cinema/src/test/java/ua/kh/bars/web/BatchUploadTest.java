package ua.kh.bars.web;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.AnnotationConfigWebContextLoader;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;
import ua.kh.bars.config.RootConfig;

import java.io.File;
import java.io.FileInputStream;

import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

/**
 * Created by green on 3/20/16.
 */

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = {RootConfig.class}, loader=AnnotationConfigWebContextLoader.class)
@WebAppConfiguration
public class BatchUploadTest {

    @Autowired
    private WebApplicationContext webApplicationContext;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        this.mockMvc = MockMvcBuilders.webAppContextSetup(this.webApplicationContext).build();
    }

    @Test
    public void uploadFile() throws Exception {
        File file1 = new File(this.getClass().getResource("/dataForUpload/batch1.json").getFile());
        File file2 = new File(this.getClass().getResource("/dataForUpload/batch2.json").getFile());
        FileInputStream fi1 = new FileInputStream(file1);
        FileInputStream fi2 = new FileInputStream(file2);
        MockMultipartFile mmpf1 = new MockMultipartFile("file", file1.getName(), "multipart/form-data", fi1);
        MockMultipartFile mmpf2 = new MockMultipartFile("file", file2.getName(), "multipart/form-data", fi2);
        this.mockMvc.perform(MockMvcRequestBuilders.fileUpload("/upload")
                .file(mmpf1)
                .file(mmpf2))
                .andExpect(status().isOk())
                .andExpect(MockMvcResultMatchers.view().name("batch/upload"));
    }

}
